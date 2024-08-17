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
// Produces the dependency graph node "baconTransformDecompose".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a953
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

 
class baconTransformDecompose : public MPxNode
{
public:
						baconTransformDecompose();
	virtual				~baconTransformDecompose(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static	MTypeId		id;

	// inputs
	static  MObject		inputMatrix;
	static  MObject		jointOrient;
	static  MObject		jointOrientX;
	static  MObject		jointOrientY;
	static  MObject		jointOrientZ;
	static  MObject		parentInverseMatrix;

	// ouputs
	static  MObject		EulerRotation;
	static  MObject		EulerRotationX;
	static  MObject		EulerRotationY;
	static  MObject		EulerRotationZ;

};

MTypeId     baconTransformDecompose::id(0x0012a953);
MObject		baconTransformDecompose::inputMatrix;
MObject		baconTransformDecompose::jointOrient;
MObject		baconTransformDecompose::jointOrientX;
MObject		baconTransformDecompose::jointOrientY;
MObject		baconTransformDecompose::jointOrientZ;
MObject		baconTransformDecompose::EulerRotation;
MObject		baconTransformDecompose::EulerRotationX;
MObject		baconTransformDecompose::EulerRotationY;
MObject		baconTransformDecompose::EulerRotationZ;
MObject		baconTransformDecompose::parentInverseMatrix;

baconTransformDecompose::baconTransformDecompose() {}
baconTransformDecompose::~baconTransformDecompose() {}


MStatus baconTransformDecompose::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if
	( 
		plug == EulerRotation	||
		plug == EulerRotationX	|| plug == EulerRotationY		|| plug == EulerRotationZ
	)
	{
		// Handles and Values
		MDataHandle inputMatrixHandle = data.inputValue(inputMatrix, &returnStatus);
		MFloatMatrix tFM(inputMatrixHandle.asFloatMatrix());

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();

		MDataHandle parentInverseMatrixHandle = data.inputValue(parentInverseMatrix, &returnStatus);
		MFloatMatrix pFM = parentInverseMatrixHandle.asFloatMatrix();

		// Calculation
		MMatrix tTM = FloatMatrixToMatrix(tFM);
		MMatrix ipTM = FloatMatrixToMatrix(pFM);
		MMatrix localTM = tTM * ipTM;
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
			// EulerRotationX
			MDataHandle EulerRotationXHandle = data.outputValue(baconTransformDecompose::EulerRotationX);
			EulerRotationXHandle.setMAngle(MAngle(outputAngles.x));
			EulerRotationXHandle.setClean();
			// EulerRotationY
			MDataHandle EulerRotationYHandle = data.outputValue(baconTransformDecompose::EulerRotationY);
			EulerRotationYHandle.setMAngle(MAngle(outputAngles.y));
			EulerRotationYHandle.setClean();
			// EulerRotationX
			MDataHandle EulerRotationZHandle = data.outputValue(baconTransformDecompose::EulerRotationZ);
			EulerRotationZHandle.setMAngle(MAngle(outputAngles.z));
			EulerRotationZHandle.setClean();
		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconTransformDecompose::creator()
{
	return new baconTransformDecompose();
}


MStatus baconTransformDecompose::initialize()
{
	MFnNumericAttribute numAttr;
	MFnUnitAttribute	uAttr;
	MFnMatrixAttribute	matrixAttr;
	MStatus				stat;

	// INPUTS ---------------------------------------------------------------------


	// input parentMatrix
	inputMatrix = matrixAttr.create("inputMatrix", "inTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(inputMatrix);

	// input parentInverseMatrix
	parentInverseMatrix = matrixAttr.create("parentInverseMatrix", "piTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentInverseMatrix);

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





	// OUTUTS ---------------------------------------------------------------------

	// Output rotationX
	EulerRotationX = uAttr.create("EulerRotationX", "rotX", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output rotationY
	EulerRotationY = uAttr.create("EulerRotationY", "rotY", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output rotationZ
	EulerRotationZ = uAttr.create("EulerRotationZ", "rotZ", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output rotation
	EulerRotation = numAttr.create("EulerRotation", "rot", EulerRotationX, EulerRotationY, EulerRotationZ);
	numAttr.setHidden(false);
	stat = addAttribute(EulerRotation);

	//AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] =	{ EulerRotation, EulerRotationX, EulerRotationY, EulerRotationZ	};
	for (MObject& obj : AffectedByMany) 
	{
		attributeAffects(jointOrient,			obj);
		attributeAffects(jointOrientX,			obj);
		attributeAffects(jointOrientY,			obj);
		attributeAffects(jointOrientZ,			obj);
		attributeAffects(inputMatrix,			obj);
		attributeAffects(parentInverseMatrix,	obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode( "baconTransformDecompose", baconTransformDecompose::id, baconTransformDecompose::creator,
								  baconTransformDecompose::initialize );
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

	status = plugin.deregisterNode( baconTransformDecompose::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
