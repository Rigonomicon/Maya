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
	static  MObject		offsetMatrix;
	static	MObject		normalizeScale;
	static	MObject		maintainOffset;

	// ouputs
	static  MObject		position;
	static  MObject		positionX;
	static  MObject		positionY;
	static  MObject		positionZ;
	static  MObject		scale;
	static  MObject		rotation;
	static  MObject		rotationX;
	static  MObject		rotationY;
	static  MObject		rotationZ;

};

MTypeId     baconTransformDecompose::id(0x0012a953);
MObject		baconTransformDecompose::inputMatrix;
MObject		baconTransformDecompose::jointOrient;
MObject		baconTransformDecompose::jointOrientX;
MObject		baconTransformDecompose::jointOrientY;
MObject		baconTransformDecompose::jointOrientZ;
MObject		baconTransformDecompose::position;
MObject		baconTransformDecompose::positionX;
MObject		baconTransformDecompose::positionY;
MObject		baconTransformDecompose::positionZ;
MObject		baconTransformDecompose::scale;
MObject		baconTransformDecompose::rotation;
MObject		baconTransformDecompose::rotationX;
MObject		baconTransformDecompose::rotationY;
MObject		baconTransformDecompose::rotationZ;
MObject		baconTransformDecompose::parentInverseMatrix;
MObject		baconTransformDecompose::offsetMatrix;
MObject		baconTransformDecompose::normalizeScale;
MObject		baconTransformDecompose::maintainOffset;

baconTransformDecompose::baconTransformDecompose() {}
baconTransformDecompose::~baconTransformDecompose() {}

static MMatrix setRow( MMatrix matrix, MVector newVector, const int row)
{
	MMatrix returnTM = matrix;
	returnTM[row][0] = newVector[0];
	returnTM[row][1] = newVector[1];
	returnTM[row][2] = newVector[2];
	return returnTM;
}

static MMatrix transMatrix(MVector pos)
{
	MMatrix returnTM = setRow(MMatrix(), pos, 3);
	return returnTM;
}

static  MMatrix FloatMatrixToMatrix(MFloatMatrix fTM)
{
	MMatrix returnTM = MMatrix();
	returnTM = setRow(returnTM, MVector(fTM[0][0], fTM[0][1], fTM[0][2]), 0);
	returnTM = setRow(returnTM, MVector(fTM[1][0], fTM[1][1], fTM[1][2]), 1);
	returnTM = setRow(returnTM, MVector(fTM[2][0], fTM[2][1], fTM[2][2]), 2);
	returnTM = setRow(returnTM, MVector(fTM[3][0], fTM[3][1], fTM[3][2]), 3);
	return returnTM;
}

static MVector getScaleFromMM(MMatrix sTM)
{
	MVector returnScale = MVector();
	returnScale.x = MVector(sTM[0][0], sTM[0][1], sTM[0][2]).length();
	returnScale.y = MVector(sTM[1][0], sTM[1][1], sTM[1][2]).length();
	returnScale.z = MVector(sTM[2][0], sTM[2][1], sTM[2][2]).length();
	return returnScale;
}

static MMatrix normalizeScaleMM(MMatrix sTM)
{
	//MMatrix returnTM = sTM;
	MVector Row0 = MVector(sTM[0][0], sTM[0][1], sTM[0][2]);
	MVector Row1 = MVector(sTM[1][0], sTM[1][1], sTM[1][2]);
	MVector Row2 = MVector(sTM[2][0], sTM[2][1], sTM[2][2]);
	setRow(sTM, Row0.normal(), 0);
	setRow(sTM, Row1.normal(), 1);
	setRow(sTM, Row2.normal(), 2);
	return sTM;
}



MStatus baconTransformDecompose::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if
	( 
		plug == rotation	|| plug == position			|| plug == scale			||
		plug == rotationX	|| plug == rotationY		|| plug == rotationZ		||
		plug == positionX   || plug == positionY        || plug == positionZ
	)
	{
		// Handles and Values
		MDataHandle normalizeScaleHandle = data.inputValue(normalizeScale, &returnStatus);
		bool normalizeScaleValue = normalizeScaleHandle.asBool();

		MDataHandle maintainOffsetHandle = data.inputValue(maintainOffset, &returnStatus);
		bool maintainOffsetValue = maintainOffsetHandle.asBool();

		MDataHandle inputMatrixHandle = data.inputValue(inputMatrix, &returnStatus);
		MFloatMatrix inputFM(inputMatrixHandle.asFloatMatrix());
		MMatrix inputTM = FloatMatrixToMatrix(inputFM);

		MDataHandle parentInverseMatrixHandle = data.inputValue(parentInverseMatrix, &returnStatus);
		MFloatMatrix ipFM = parentInverseMatrixHandle.asFloatMatrix();
		MMatrix ipTM = FloatMatrixToMatrix(ipFM);

		MDataHandle offsetMatrixHandle = data.inputValue(offsetMatrix, &returnStatus);
		MFloatMatrix offsetFM = offsetMatrixHandle.asFloatMatrix();
		MMatrix offsetTM = FloatMatrixToMatrix(offsetFM);

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();
		MMatrix jointOrientTM = MEulerRotation(jointOrientXValue.value(), jointOrientYValue.value(), jointOrientZValue.value()).asMatrix();

		// Calculation
		if (maintainOffsetValue) { 
			inputTM = offsetTM * inputTM;
		}
		MMatrix localTM = inputTM * ipTM; // *opmTM.inverse(); offset parent matrix included in parent matrix! Why Autodesk?!
		MMatrix outputRotationTM = localTM * jointOrientTM.inverse();
		MTransformationMatrix ouputTransformationMatrix = MTransformationMatrix(outputRotationTM);
		MEulerRotation outputAngles = ouputTransformationMatrix.eulerRotation();
		MVector outScale = getScaleFromMM(localTM);

		// Set OutPut Values
		if( returnStatus != MS::kSuccess )
			cerr << "ERROR getting data" << endl;
		else
		{
			// EulerRotationX
			MDataHandle rotationXHandle = data.outputValue(baconTransformDecompose::rotationX);
			rotationXHandle.setMAngle(MAngle(outputAngles.x));
			rotationXHandle.setClean();
			// rotationY
			MDataHandle rotationYHandle = data.outputValue(baconTransformDecompose::rotationY);
			rotationYHandle.setMAngle(MAngle(outputAngles.y));
			rotationYHandle.setClean();
			// rotationX
			MDataHandle rotationZHandle = data.outputValue(baconTransformDecompose::rotationZ);
			rotationZHandle.setMAngle(MAngle(outputAngles.z));
			rotationZHandle.setClean();

			// position // Deprecated method since Maya prioritizes components and fails to update if not.
			//MDataHandle positionHandle = data.outputValue(baconTransformDecompose::position);
			//positionHandle.set3Float(localTM[3][0], localTM[3][1], localTM[3][2]);
			//positionHandle.setClean();
			MDataHandle positionXHandle = data.outputValue(baconTransformDecompose::positionX);
			positionXHandle.setFloat(localTM[3][0]);
			positionXHandle.setClean();
			MDataHandle positionYHandle = data.outputValue(baconTransformDecompose::positionY);
			positionYHandle.setFloat(localTM[3][1]);
			positionYHandle.setClean();
			MDataHandle positionZHandle = data.outputValue(baconTransformDecompose::positionZ);
			positionZHandle.setFloat(localTM[3][2]);
			positionZHandle.setClean();


			// scale
			MDataHandle scaleHandle = data.outputValue(baconTransformDecompose::scale);
			if (normalizeScaleValue) {
				scaleHandle.set3Float(1.0, 1.0, 1.0);
			} else {
				scaleHandle.set3Float(outScale.x, outScale.y, outScale.z);
			}
			scaleHandle.setClean();

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

	// normalizeScale
	normalizeScale = numAttr.create("normalizeScale", "ns", MFnNumericData::kBoolean);
	numAttr.setDefault(false);
	numAttr.setKeyable(true);
	numAttr.setReadable(true);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setConnectable(false);
	stat = addAttribute(normalizeScale);

	// maintainOffset
	maintainOffset = numAttr.create("maintainOffset", "off", MFnNumericData::kBoolean);
	numAttr.setDefault(false);
	numAttr.setKeyable(true);
	numAttr.setReadable(true);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setConnectable(false);
	stat = addAttribute(maintainOffset);


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

	// input offsetMatrix
	offsetMatrix = matrixAttr.create("offsetMatrix", "offTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(offsetMatrix);

	// offset parent matrix - removed since Maya includes it in parent matrix values
	//offsetParentMatrix = matrixAttr.create("offsetParentMatrix", "opm", matrixAttr.kFloat);
	//matrixAttr.setStorable(true);
	//matrixAttr.setKeyable(true);
	//stat = addAttribute(offsetParentMatrix);


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

	// Output position
	//position = numAttr.createPoint("position", "trn");
	//numAttr.setStorable(true);
	//numAttr.setHidden(false);
	//stat = addAttribute(position);
	positionX = numAttr.create("positionX", "trnX", MFnNumericData::kFloat, 0.0);
	uAttr.setWritable(false);
	// Output positionY
	positionY = numAttr.create("positionY", "trnY", MFnNumericData::kFloat, 0.0);
	uAttr.setWritable(false);
	// Output positionZ
	positionZ = numAttr.create("positionZ", "trnZ", MFnNumericData::kFloat, 0.0);
	uAttr.setWritable(false);
	// Output position
	position = numAttr.create("position", "trn", positionX, positionY, positionZ);
	numAttr.setHidden(false);
	stat = addAttribute(position);





	// Output rotationX
	rotationX = uAttr.create("rotationX", "rotX", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output rotationY
	rotationY = uAttr.create("rotationY", "rotY", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output rotationZ
	rotationZ = uAttr.create("rotationZ", "rotZ", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output rotation
	rotation = numAttr.create("rotation", "rot", rotationX, rotationY, rotationZ);
	numAttr.setHidden(false);
	stat = addAttribute(rotation);

	// Output scale
	scale = numAttr.createPoint("scale", "scl");
	numAttr.setDefault(1.0, 1.0, 1.0);
	numAttr.setStorable(true);
	numAttr.setHidden(false);
	stat = addAttribute(scale);




	//AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] =	{ rotation, rotationX, rotationY, rotationZ, position, positionX, positionY, positionZ, scale	};
	for (MObject& obj : AffectedByMany) 
	{
		attributeAffects(jointOrient,			obj);
		attributeAffects(jointOrientX,			obj);
		attributeAffects(jointOrientY,			obj);
		attributeAffects(jointOrientZ,			obj);
		attributeAffects(inputMatrix,			obj);
		attributeAffects(parentInverseMatrix,   obj);
		attributeAffects(offsetMatrix,			obj);
		attributeAffects(normalizeScale,		obj);
		attributeAffects(maintainOffset,		obj);
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
