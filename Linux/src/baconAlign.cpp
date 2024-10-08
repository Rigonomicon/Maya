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
// Produces the dependency graph node "baconAlign".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a950
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

 
class baconAlign : public MPxNode
{
public:
						baconAlign();
	virtual				~baconAlign(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static	MTypeId		id;

	// inputs
	static  MObject		weight;						// The weight value.
	static  MObject		mirror;
	static  MObject		targetWorldMatrix;
	static  MObject		parentWorldMatrix;
	static  MObject		offsetMatrix;
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

MTypeId     baconAlign::id( 0x0012a950 );
MObject     baconAlign::weight;
MObject     baconAlign::mirror;
MObject		baconAlign::targetWorldMatrix;
MObject		baconAlign::parentWorldMatrix;
MObject		baconAlign::offsetMatrix;
MObject		baconAlign::jointOrient;
MObject		baconAlign::jointOrientX;
MObject		baconAlign::jointOrientY;
MObject		baconAlign::jointOrientZ;
MObject		baconAlign::alignedRotation;
MObject		baconAlign::alignedRotationX;
MObject		baconAlign::alignedRotationY;
MObject		baconAlign::alignedRotationZ;
MObject		baconAlign::alignedPosition;

baconAlign::baconAlign() {}
baconAlign::~baconAlign() {}

MMatrix setRow( MMatrix matrix, MVector newVector, const int row)
{
	MMatrix returnTM = matrix;
	returnTM[row][0] = newVector[0];
	returnTM[row][1] = newVector[1];
	returnTM[row][2] = newVector[2];
	return returnTM;
}

MMatrix transMatrix(MVector pos)
{
	MMatrix returnTM = setRow(MMatrix(), pos, 3);
	return returnTM;
}

MMatrix FloatMatrixToMatrix(MFloatMatrix fTM)
{
	MMatrix returnTM = MMatrix();
	returnTM = setRow(returnTM, MVector(fTM[0][0], fTM[0][1], fTM[0][2]), 0);
	returnTM = setRow(returnTM, MVector(fTM[1][0], fTM[1][1], fTM[1][2]), 1);
	returnTM = setRow(returnTM, MVector(fTM[2][0], fTM[2][1], fTM[2][2]), 2);
	returnTM = setRow(returnTM, MVector(fTM[3][0], fTM[3][1], fTM[3][2]), 3);
	return returnTM;
}

MMatrix mirrorMatrix(MMatrix matrix)
{
	MMatrix returnTM = matrix;
	returnTM[0][0] = returnTM[0][0] * -1.0;
	returnTM[0][1] = returnTM[0][1] * -1.0;
	returnTM[0][2] = returnTM[0][2] * -1.0;
	returnTM[1][0] = returnTM[1][0] * -1.0;
	returnTM[1][1] = returnTM[1][1] * -1.0;
	returnTM[1][2] = returnTM[1][2] * -1.0;
	returnTM[2][0] = returnTM[2][0] * -1.0;
	returnTM[2][1] = returnTM[2][1] * -1.0;
	returnTM[2][2] = returnTM[2][2] * -1.0;
	returnTM[3][0] = returnTM[3][0] * -1.0;
	returnTM[3][1] = returnTM[3][1] * -1.0;
	returnTM[3][2] = returnTM[3][2] * -1.0;
	return returnTM;
}


MStatus baconAlign::compute( const MPlug& plug, MDataBlock& data )
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

		MDataHandle parentWorldMatrixHandle = data.inputValue(parentWorldMatrix, &returnStatus);
		MFloatMatrix pFM = parentWorldMatrixHandle.asFloatMatrix();

		MDataHandle offsetMatrixHandle = data.inputValue(offsetMatrix, &returnStatus);
		MFloatMatrix offsetFM = offsetMatrixHandle.asFloatMatrix();

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();

		MDataHandle mirrorHandle = data.inputValue(mirror, &returnStatus);
		bool mirrorValue = mirrorHandle.asBool();

		// Calculation
		MMatrix targetTM = FloatMatrixToMatrix(tFM);
		MMatrix parentTM = FloatMatrixToMatrix(pFM);
		MMatrix offsetTM = FloatMatrixToMatrix(offsetFM);
		MMatrix worldTM = offsetTM * targetTM;
		if (mirrorValue)
		{
			worldTM = mirrorMatrix(worldTM);
		}
		MMatrix localTM = worldTM * parentTM.inverse();
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
			// alignedRotationX
			MDataHandle alignedRotationXHandle = data.outputValue(baconAlign::alignedRotationX);
			alignedRotationXHandle.setMAngle(MAngle(outputAngles.x));
			alignedRotationXHandle.setClean();
			// alignedRotationY
			MDataHandle alignedRotationYHandle = data.outputValue(baconAlign::alignedRotationY);
			alignedRotationYHandle.setMAngle(MAngle(outputAngles.y));
			alignedRotationYHandle.setClean();
			// alignedRotationX
			MDataHandle alignedRotationZHandle = data.outputValue(baconAlign::alignedRotationZ);
			alignedRotationZHandle.setMAngle(MAngle(outputAngles.z));
			alignedRotationZHandle.setClean();

			// aligned Position
			MDataHandle alignedPositionHandle = data.outputValue(baconAlign::alignedPosition);
			alignedPositionHandle.set3Float(localTM[3][0], localTM[3][1], localTM[3][2]);
			alignedPositionHandle.setClean();

		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconAlign::creator()
{
	return new baconAlign();
}


MStatus baconAlign::initialize()
{
	MFnNumericAttribute numAttr;
	MFnUnitAttribute	uAttr;
	MFnMatrixAttribute	matrixAttr;
	MFnNumericAttribute nAttr;
	MStatus				stat;

	// INPUTS ---------------------------------------------------------------------
	
	// weight
	weight = numAttr.create("weight", "w", MFnNumericData::kFloat, 0.0);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	
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

	// input offsetMatrix
	offsetMatrix = matrixAttr.create("offsetMatrix", "offsetTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(offsetMatrix);

	// input targetWorldMatrix
	targetWorldMatrix = matrixAttr.create("targetWorldMatrix", "twTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(targetWorldMatrix);

	// input parentWorldMatrix
	parentWorldMatrix = matrixAttr.create("parentWorldMatrix", "pwTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentWorldMatrix);

	// mirror
	mirror = nAttr.create("mirror", "mir", MFnNumericData::kBoolean);
	nAttr.setDefault(false);
	nAttr.setKeyable(false);
	nAttr.setReadable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(mirror);
	if (!stat) { stat.perror("addAttribute"); return stat; }


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
		attributeAffects(mirror,			obj);
		attributeAffects(jointOrient,		obj);
		attributeAffects(jointOrientX,		obj);
		attributeAffects(jointOrientY,		obj);
		attributeAffects(jointOrientZ,		obj);
		attributeAffects(targetWorldMatrix,	obj);
		attributeAffects(parentWorldMatrix,	obj);
		attributeAffects(offsetMatrix,		obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode( "baconAlign", baconAlign::id, baconAlign::creator,
								  baconAlign::initialize );
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

	status = plugin.deregisterNode( baconAlign::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
