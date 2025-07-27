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
// Produces the dependency graph node "baconPreheat".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a964
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <array>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h> 

#include <maya/MFnMessageAttribute.h>
#include <maya/MFnCompoundAttribute.h>
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
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFloatPoint.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MScriptUtil.h>

#include "baconMath.h"

 
class baconPreheat : public MPxNode
{
public:
						baconPreheat();
	virtual				~baconPreheat(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static	MTypeId		id;

	// inputs
	static  MObject		weight;						// The weight value.
	static  MObject		bakePosition;
	static  MObject		bakeRotation;
	static  MObject		driverNode;
	static  MObject		drivenNode;
	static  MObject		targetWorldMatrix;
	static  MObject		offsetMatrix;
	static  MObject		parentInverseMatrix;
	static  MObject		jointOrient;
	static  MObject		jointOrientX;
	static  MObject		jointOrientY;
	static  MObject		jointOrientZ;

	// ouputs
	static  MObject		alignedRotation;
	static  MObject		alignedRotationX;
	static  MObject		alignedRotationY;
	static  MObject		alignedRotationZ;
	static  MObject		alignedPosition;

};

MTypeId     baconPreheat::id( 0x0012a964 );
MObject     baconPreheat::weight;        
MObject     baconPreheat::bakePosition;        
MObject     baconPreheat::bakeRotation;
MObject     baconPreheat::driverNode;
MObject     baconPreheat::drivenNode;
MObject		baconPreheat::targetWorldMatrix;
MObject		baconPreheat::offsetMatrix;
MObject		baconPreheat::parentInverseMatrix;
MObject		baconPreheat::jointOrient;
MObject		baconPreheat::jointOrientX;
MObject		baconPreheat::jointOrientY;
MObject		baconPreheat::jointOrientZ;
MObject		baconPreheat::alignedRotation;
MObject		baconPreheat::alignedRotationX;
MObject		baconPreheat::alignedRotationY;
MObject		baconPreheat::alignedRotationZ;
MObject		baconPreheat::alignedPosition;

baconPreheat::baconPreheat() {}
baconPreheat::~baconPreheat() {}


MStatus baconPreheat::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if
	( 
		plug == alignedRotation		|| plug == alignedPosition		||
		plug == alignedRotationX	|| plug == alignedRotationY		|| plug == alignedRotationZ
	)
	{
		// Handles and Values
		MDataHandle targetWorldMatrixHandle = data.inputValue(targetWorldMatrix, &returnStatus);
		MFloatMatrix tFM(targetWorldMatrixHandle.asFloatMatrix());

		MDataHandle offsetMatrixHandle = data.inputValue(offsetMatrix, &returnStatus);
		MFloatMatrix offsetFM(offsetMatrixHandle.asFloatMatrix());

		MDataHandle parentInverseMatrixHandle = data.inputValue(parentInverseMatrix, &returnStatus);
		MFloatMatrix pFM = parentInverseMatrixHandle.asFloatMatrix();

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();


		// Calculation
		MMatrix tTM = FloatMatrixToMatrix(tFM);
		MMatrix offsetTM = FloatMatrixToMatrix(offsetFM);
		MMatrix ipTM = FloatMatrixToMatrix(pFM);
		MMatrix localTM = (offsetTM * tTM) * ipTM;
		MMatrix jointOrientTM = MEulerRotation(jointOrientXValue.value(), jointOrientYValue.value(),
			jointOrientZValue.value()).asMatrix();
		MMatrix outputTM = localTM * jointOrientTM.inverse();
		MTransformationMatrix ouputTransformationMatrix = MTransformationMatrix(outputTM);
		MEulerRotation outputAngles = ouputTransformationMatrix.eulerRotation();


		// Set OutPut Values
		if( returnStatus != MS::kSuccess )
			cerr << "ERROR getting data" << endl;
		else
		{
			// float result = sinf(weightData.asFloat()) * 10.0f;
			// MAngle uX = jointOrientXHandle.asAngle();
			//MAngle uX = jointOrientXHandle.asDouble();
			//double result = uX.;

			//humerusLengthHandle = inputValue.asFloat(bio2BoneIK.humerusLength)
			//SideA = humerusLengthHandle.asFloat()


			// weight
			//MDataHandle weightHandle = data.outputValue(baconPreheat::weight);
			//weightHandle.setDouble();


			// alignedRotationX
			MDataHandle alignedRotationXHandle = data.outputValue(baconPreheat::alignedRotationX);
			alignedRotationXHandle.setMAngle(MAngle(outputAngles.x));
			alignedRotationXHandle.setClean();
			// alignedRotationY
			MDataHandle alignedRotationYHandle = data.outputValue(baconPreheat::alignedRotationY);
			alignedRotationYHandle.setMAngle(MAngle(outputAngles.y));
			alignedRotationYHandle.setClean();
			// alignedRotationX
			MDataHandle alignedRotationZHandle = data.outputValue(baconPreheat::alignedRotationZ);
			alignedRotationZHandle.setMAngle(MAngle(outputAngles.z));
			alignedRotationZHandle.setClean();

			// aligned Position
			MDataHandle alignedPositionHandle = data.outputValue(baconPreheat::alignedPosition);
			alignedPositionHandle.set3Float(localTM[3][0], localTM[3][1], localTM[3][2]);
				//setMVector(MVector(outputTM[3][0], outputTM[3][1], outputTM[3][2]));
			alignedPositionHandle.setClean();

		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconPreheat::creator()
{
	return new baconPreheat();
}


MStatus baconPreheat::initialize()
{
	MFnMessageAttribute mAttr;
	MFnCompoundAttribute compAttr;
	MFnNumericAttribute numAttr;
	MFnUnitAttribute	uAttr;
	MFnMatrixAttribute	matrixAttr;
	MStatus				stat;

	// INPUTS ---------------------------------------------------------------------
	
	// weight
	weight = numAttr.create("weight", "w", MFnNumericData::kFloat, 0.0);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	stat = addAttribute(bakePosition);

	// bakePosition
	bakePosition = numAttr.create("bakePosition", "bpos", MFnNumericData::kBoolean);
	numAttr.setDefault(false);
	numAttr.setKeyable(false);
	numAttr.setReadable(true);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	stat = addAttribute(bakePosition);

	// bakeRotation
	bakeRotation = numAttr.create("bakeRotation", "brot", MFnNumericData::kBoolean);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	stat = addAttribute(bakeRotation);

	// driverNode
	driverNode = mAttr.create("driverNode", "drn");
	mAttr.setWritable(true);
	mAttr.setStorable(true);
	mAttr.setKeyable(true);
	stat = addAttribute(driverNode);

	// drivenNode
	drivenNode = mAttr.create("drivenNode", "dnn");
	mAttr.setWritable(true);
	mAttr.setStorable(true);
	mAttr.setKeyable(true);
	stat = addAttribute(drivenNode);

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

	// input offsetMatrix
	offsetMatrix = matrixAttr.create("offsetMatrix", "offTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(offsetMatrix);

	// input parentInverseMatrix
	parentInverseMatrix = matrixAttr.create("parentInverseMatrix", "piTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentInverseMatrix);


	// OUTUTS ---------------------------------------------------------------------

	// Output aligned rotationX
	alignedRotationX = uAttr.create("alignedRotationX", "arotX", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output aligned rotationY
	alignedRotationY = uAttr.create("alignedRotationY", "arotY", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output aligned rotationZ
	alignedRotationZ = uAttr.create("alignedRotationZ", "arotZ", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output aligned rotation
	alignedRotation = numAttr.create("alignedRotation", "arot", alignedRotationX, alignedRotationY, alignedRotationZ);
	numAttr.setHidden(false);
	stat = addAttribute(alignedRotation);

	// Output Aligned Position
	alignedPosition = numAttr.createPoint("alignedPosition", "apos");
	numAttr.setStorable(false);
	numAttr.setHidden(false);
	stat = addAttribute(alignedPosition);

	
	//AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] =
	{	alignedRotation,	alignedPosition,
		alignedRotationX,	alignedRotationY,	alignedRotationZ
	};
	for (MObject& obj : AffectedByMany) 
	{
		attributeAffects(jointOrient,			obj);
		attributeAffects(jointOrientX,			obj);
		attributeAffects(jointOrientY,			obj);
		attributeAffects(jointOrientZ,			obj);
		attributeAffects(targetWorldMatrix,		obj);
		attributeAffects(offsetMatrix,			obj);
		attributeAffects(parentInverseMatrix,	obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "CGRigging - XRIG", "3.0", "Any");

	status = plugin.registerNode( "baconPreheat", baconPreheat::id, baconPreheat::creator,
								  baconPreheat::initialize );
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

	status = plugin.deregisterNode( baconPreheat::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
