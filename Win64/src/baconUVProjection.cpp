//-
// ==========================================================================
// BACON-STRIP.com RIG
//
// MIT License
// Copyright (c) 2017 Bacon-Strip
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// Produces the dependency graph node "baconUVProjection".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a965
//
////////////////////////////////////////////////////////////////////////



#include <string.h>
#include <array>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h> 

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnPlugin.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MAngle.h>
#include <maya/MEulerRotation.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFloatPoint.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MScriptUtil.h>

#include "baconMath.h"

 
class baconUVProjection : public MPxNode
{
public:
						baconUVProjection();
	virtual				~baconUVProjection(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static	MTypeId		id;

	// inputs
	static  MObject		targetWorldMatrix;
	static  MObject		parentWorldMatrix;
	static  MObject		referenceWorldMatrix;
	static  MObject		jointOrient;
	static  MObject		jointOrientX;
	static  MObject		jointOrientY;
	static  MObject		jointOrientZ;
	static  MObject		UVOffset;
	static  MObject		aimAxis;
	static  MObject		aimAxisFlip;



	// ouputs
	static  MObject		U;
	static  MObject		V;
	static  MObject		UV;

};

MTypeId     baconUVProjection::id( 0x0012a965 );
MObject		baconUVProjection::targetWorldMatrix;
MObject		baconUVProjection::parentWorldMatrix;
MObject		baconUVProjection::referenceWorldMatrix;
MObject		baconUVProjection::jointOrient;
MObject		baconUVProjection::jointOrientX;
MObject		baconUVProjection::jointOrientY;
MObject		baconUVProjection::jointOrientZ;
MObject		baconUVProjection::UVOffset;
MObject		baconUVProjection::aimAxis;
MObject		baconUVProjection::aimAxisFlip;
MObject		baconUVProjection::U;
MObject		baconUVProjection::V;
MObject		baconUVProjection::UV;

baconUVProjection::baconUVProjection() {}
baconUVProjection::~baconUVProjection() {}


MStatus baconUVProjection::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if
	( 
		plug == UV || plug == U || plug == V
	)
	{
		// Handles and Values
		MDataHandle targetWorldMatrixHandle = data.inputValue(targetWorldMatrix, &returnStatus);
		MFloatMatrix tFM(targetWorldMatrixHandle.asFloatMatrix());
		MDataHandle parentWorldMatrixHandle = data.inputValue(parentWorldMatrix, &returnStatus);
		MFloatMatrix pFM = parentWorldMatrixHandle.asFloatMatrix();
		MDataHandle referenceWorldMatrixHandle = data.inputValue(referenceWorldMatrix, &returnStatus);
		MFloatMatrix unFM = referenceWorldMatrixHandle.asFloatMatrix();

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();


		MDataHandle aimAxisHandle = data.inputValue(aimAxis, &returnStatus);
		short aimAxisValue = aimAxisHandle.asShort();
		MDataHandle aimAxisFlipHandle = data.inputValue(aimAxisFlip, &returnStatus);
		bool aimAxisFlipValue = aimAxisFlipHandle.asBool();

		MDataHandle UVOffsetHandle = data.inputValue(UVOffset, &returnStatus);
		MVector UVOffsetValue = UVOffsetHandle.asFloatVector();


		// Calculation
		MMatrix targetTM = FloatMatrixToMatrix(tFM);
		MMatrix parentTM = FloatMatrixToMatrix(pFM);
		MMatrix referenceTM = FloatMatrixToMatrix(unFM);

		// AIM Direction
		MVector aimDir;
		if (aimAxisValue == 0) aimDir = MVector(referenceTM[0][0], referenceTM[0][1], referenceTM[0][2]);
		if (aimAxisValue == 1) aimDir = MVector(referenceTM[1][0], referenceTM[1][1], referenceTM[1][2]);
		if (aimAxisValue == 2) aimDir = MVector(referenceTM[2][0], referenceTM[2][1], referenceTM[2][2]);
		if (aimAxisFlipValue) aimDir.operator*=(-1.0);

		// Cheap Projection

		MVector n(targetTM[2][0], targetTM[2][1], targetTM[2][2]);					//normal of plane should be z-axis
		MVector nn(-1.0 * n);														//negative normal of plane should be z-axis
		MVector eyePos(referenceTM[3][0], referenceTM[3][1], referenceTM[3][2]);	//World Position of eyeball
		MVector planePos(targetTM[3][0], targetTM[3][1], targetTM[3][2]);			//World Position of plane
		MVector w = eyePos - planePos;

		double prod1 = w * n;
		double prod2 = aimDir * n;
		double prod3 = prod1 / prod2;

		MVector IntersectPoint = eyePos - aimDir * prod3;

		/*
		
		Vector3D diff = rayPoint - planePoint;
		double prod1 = diff.dot(planeNormal);
		double prod2 = rayVector.dot(planeNormal);
		double prod3 = prod1 / prod2;
		return rayPoint - rayVector * prod3;
		
		*/


		MMatrix aimTM = transMatrix(IntersectPoint);
		MMatrix localTM = aimTM * targetTM.inverse();

		// Set OutPut Values
		if( returnStatus != MS::kSuccess )
			cerr << "ERROR getting data" << endl;
		else
		{
			MDataHandle UVHandle = data.outputValue(baconUVProjection::UV);
			UVHandle.set2Double(localTM[3][0], localTM[3][1]);
			UVHandle.setClean();
		}
		
	
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconUVProjection::creator()
{
	return new baconUVProjection();
}


MStatus baconUVProjection::initialize()
{
	MFnNumericAttribute numAttr;
	MFnUnitAttribute	uAttr;
	MFnMatrixAttribute	matrixAttr;
	MFnEnumAttribute	enumAttr;
	MStatus				stat;

	// INPUTS ---------------------------------------------------------------------
	
	// aimAxis
	aimAxis = enumAttr.create("aimAxis", "aima", 0);
	enumAttr.addField("X", 0);
	enumAttr.addField("Y", 1);
	enumAttr.addField("Z", 2);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(aimAxis);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	// aimAxisFlip
	aimAxisFlip = numAttr.create("aimAxisFlip", "aaf", MFnNumericData::kBoolean);
	numAttr.setDefault(false);
	numAttr.setKeyable(true);
	numAttr.setReadable(true);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	stat = addAttribute(aimAxisFlip);

	// UVOffset
	UVOffset = numAttr.createPoint("UVOffset", "lpos");
	numAttr.setStorable(true);
	numAttr.setWritable(true);
	numAttr.setKeyable(true);
	stat = addAttribute(UVOffset);

	// input jointOrientX
	jointOrientX = uAttr.create("jointOrientX", "uox", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// input jointOrientY
	jointOrientY = uAttr.create("jointOrientY", "uoy", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// input jointOrientZ
	jointOrientZ = uAttr.create("jointOrientZ", "uoz", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// input jointOrient
	jointOrient = numAttr.create("jointOrient", "jo", jointOrientX, jointOrientY, jointOrientZ);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	stat = addAttribute(jointOrient);

	// input targetWorldMatrix
	targetWorldMatrix = matrixAttr.create("targetWorldMatrix", "twTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(targetWorldMatrix);

	// input parentWorldMatrix
	parentWorldMatrix = matrixAttr.create("parentWorldMatrix", "piTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentWorldMatrix);

	// input referenceWorldMatrix
	referenceWorldMatrix = matrixAttr.create("referenceWorldMatrix", "refTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(referenceWorldMatrix);




	// OUTUTS ---------------------------------------------------------------------

	U = numAttr.create("uCoord", "u", MFnNumericData::kDouble, 0.0);
	V = numAttr.create("vCoord", "v", MFnNumericData::kDouble, 0.0);
	UV = numAttr.create("UV", "uv", U, V);
	numAttr.setWritable(false);
	stat = addAttribute(UV);

	//AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] =
	{	
		UV, U, V
	};

	for (MObject& obj : AffectedByMany) 
	{
		attributeAffects(jointOrient,		obj);
		attributeAffects(jointOrientX,		obj);
		attributeAffects(jointOrientY,		obj);
		attributeAffects(jointOrientZ,		obj);
		attributeAffects(targetWorldMatrix,	obj);
		attributeAffects(parentWorldMatrix, obj);
		attributeAffects(referenceWorldMatrix, obj);
		attributeAffects(aimAxis,			obj);
		attributeAffects(aimAxisFlip,		obj);
		attributeAffects(UVOffset,			obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode( "baconUVProjection", baconUVProjection::id, baconUVProjection::creator,
								  baconUVProjection::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( baconUVProjection::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
