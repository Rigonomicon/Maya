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
// Produces the dependency graph node "baconMatrixCompose".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a962
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
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFloatPoint.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MScriptUtil.h>

#include "baconMath.h"

 
class baconMatrixCompose : public MPxNode
{
public:
						baconMatrixCompose();
	virtual				~baconMatrixCompose(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static	MTypeId		id;

	// inputs
	static  MObject		translation;
	static  MObject		EulerRotation;
	static  MObject		EulerRotationX;
	static  MObject		EulerRotationY;
	static  MObject		EulerRotationZ;
	static  MObject		scale;
	static  MObject		jointOrient;
	static  MObject		jointOrientX;
	static  MObject		jointOrientY;
	static  MObject		jointOrientZ;
	static  MObject		offsetParentMatrix;

	// ouputs
	static  MObject		outputMatrix;

};

MTypeId     baconMatrixCompose::id(0x0012a962);
MObject		baconMatrixCompose::outputMatrix;
MObject		baconMatrixCompose::translation;
MObject		baconMatrixCompose::scale;
MObject		baconMatrixCompose::EulerRotation;
MObject		baconMatrixCompose::EulerRotationX;
MObject		baconMatrixCompose::EulerRotationY;
MObject		baconMatrixCompose::EulerRotationZ;
MObject		baconMatrixCompose::jointOrient;
MObject		baconMatrixCompose::jointOrientX;
MObject		baconMatrixCompose::jointOrientY;
MObject		baconMatrixCompose::jointOrientZ;
MObject		baconMatrixCompose::offsetParentMatrix;

baconMatrixCompose::baconMatrixCompose() {}
baconMatrixCompose::~baconMatrixCompose() {}

MStatus baconMatrixCompose::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if
	( 
		plug == outputMatrix 
	)
	{
		// Handles and Values

		MDataHandle translationHandle = data.inputValue(translation, &returnStatus);
		MVector translationValue = translationHandle.asFloatVector();

		MDataHandle scaleHandle = data.inputValue(scale, &returnStatus);
		MVector scaleValue = scaleHandle.asFloatVector();

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();

		MDataHandle EulerRotationXHandle = data.inputValue(EulerRotationX, &returnStatus);
		MAngle EulerRotationXValue = EulerRotationXHandle.asAngle();
		MDataHandle EulerRotationYHandle = data.inputValue(EulerRotationY, &returnStatus);
		MAngle EulerRotationYValue = EulerRotationYHandle.asAngle();
		MDataHandle EulerRotationZHandle = data.inputValue(EulerRotationZ, &returnStatus);
		MAngle EulerRotationZValue = EulerRotationZHandle.asAngle();

		MDataHandle offsetParentMatrixHandle = data.inputValue(offsetParentMatrix, &returnStatus);
		MFloatMatrix offsetParentTM = offsetParentMatrixHandle.asFloatMatrix();

		// Calculation
		MMatrix transTM = transMatrix(translationValue);
		MMatrix scaleTM = scaleMatrix(scaleValue);
		MMatrix OPM = FloatMatrixToMatrix(offsetParentTM);
		MMatrix jointOrientTM = MEulerRotation(jointOrientXValue.value(), jointOrientYValue.value(),
			jointOrientZValue.value()).asMatrix();
		MMatrix EulerRotationTM = MEulerRotation(EulerRotationXValue.value(), EulerRotationYValue.value(),
			EulerRotationZValue.value()).asMatrix();

		MMatrix outputTM = OPM * jointOrientTM * scaleTM * EulerRotationTM * transTM;

		// Set OutPut Values
		if( returnStatus != MS::kSuccess )
			cerr << "ERROR getting data" << endl;
		else
		{
			// Output Matrix
			MDataHandle outputMatrixHandle = data.outputValue(baconMatrixCompose::outputMatrix);
			outputMatrixHandle.setMFloatMatrix(MMatrixToFloatMatrix(outputTM));
			outputMatrixHandle.setClean();
			
		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconMatrixCompose::creator()
{
	return new baconMatrixCompose();
}


MStatus baconMatrixCompose::initialize()
{
	MFnNumericAttribute numAttr;
	MFnUnitAttribute	uAttr;
	MFnMatrixAttribute	matrixAttr;
	MStatus				stat;

	// INPUTS ---------------------------------------------------------------------

	// Translation
	translation = numAttr.createPoint("translation", "tr");
	numAttr.setDefault(0.0, 0.0, 0.0);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	stat = addAttribute(translation);

	// rotationX
	EulerRotationX = uAttr.create("EulerRotationX", "rotX", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// rotationY
	EulerRotationY = uAttr.create("EulerRotationY", "rotY", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// rotationZ
	EulerRotationZ = uAttr.create("EulerRotationZ", "rotZ", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// rotation
	EulerRotation = numAttr.create("EulerRotation", "rot", EulerRotationX, EulerRotationY, EulerRotationZ);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	stat = addAttribute(EulerRotation);

	// scale
	scale = numAttr.createPoint("scale", "sc");
	numAttr.setDefault(1.0, 1.0, 1.0);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	stat = addAttribute(scale);

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

	// input offsetParentMatrix
	offsetParentMatrix = matrixAttr.create("offsetParentMatrix", "opm", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(offsetParentMatrix);



	// OUTUTS ---------------------------------------------------------------------

	// outputMatrix
	outputMatrix = matrixAttr.create("outputMatrix", "outTM", matrixAttr.kFloat);
	matrixAttr.setWritable(false);
	stat = addAttribute(outputMatrix);



	//AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] =	{ outputMatrix };
	for (MObject& obj : AffectedByMany) 
	{
		attributeAffects(translation, obj);
		attributeAffects(scale, obj);
		attributeAffects(jointOrient, obj);
		attributeAffects(jointOrientX, obj);
		attributeAffects(jointOrientY, obj);
		attributeAffects(jointOrientZ, obj);
		attributeAffects(EulerRotation, obj);
		attributeAffects(EulerRotationX, obj);
		attributeAffects(EulerRotationY, obj);
		attributeAffects(EulerRotationZ, obj);
		attributeAffects(offsetParentMatrix,	obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode( "baconMatrixCompose", baconMatrixCompose::id, baconMatrixCompose::creator,
								  baconMatrixCompose::initialize );
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

	status = plugin.deregisterNode( baconMatrixCompose::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
