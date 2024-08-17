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
// Produces the dependency graph node "baconDoubleBezier".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a966
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <array>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h> 
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
#include <maya/MQuaternion.h>

#include <maya/MFloatPoint.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MScriptUtil.h>

 
class baconDoubleBezier : public MPxNode
{
public:
						baconDoubleBezier();
	virtual				~baconDoubleBezier(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static	MTypeId		id;

	// inputs
	static  MObject		Curve1Percent;						// The Curve1Percent value for the start.
	static  MObject		Curve2Percent;							

	static  MObject		compCurve1;
	static  MObject		compCurve2;

	static  MObject		startCurve1WorldMatrix;
	static  MObject		endCurve1WorldMatrix;
	static  MObject		startCurve2WorldMatrix;
	static  MObject		endCurve2WorldMatrix;

	static  MObject		startTangent1WorldMatrix;
	static  MObject		endTangent1WorldMatrix;
	static  MObject		startTangent2WorldMatrix;
	static  MObject		endTangent2WorldMatrix;

	static  MObject		parentInverseMatrix;
	static  MObject		jointOrient;
	static  MObject		jointOrientX;
	static  MObject		jointOrientY;
	static  MObject		jointOrientZ;

	static  MObject		spin;
	static  MObject		spinStart;
	static  MObject		spinEnd;


	// ouputs
	static  MObject		Rotation;
	static  MObject		RotationX;
	static  MObject		RotationY;
	static  MObject		RotationZ;
	static  MObject		Position;




};

MTypeId     baconDoubleBezier::id( 0x0012a966 );
MObject     baconDoubleBezier::Curve1Percent;
MObject     baconDoubleBezier::Curve2Percent;

MObject     baconDoubleBezier::compCurve1;
MObject     baconDoubleBezier::compCurve2;

MObject		baconDoubleBezier::startCurve1WorldMatrix;
MObject		baconDoubleBezier::endCurve1WorldMatrix;
MObject		baconDoubleBezier::startCurve2WorldMatrix;
MObject		baconDoubleBezier::endCurve2WorldMatrix;

MObject		baconDoubleBezier::startTangent1WorldMatrix;
MObject		baconDoubleBezier::endTangent1WorldMatrix;
MObject		baconDoubleBezier::startTangent2WorldMatrix;
MObject		baconDoubleBezier::endTangent2WorldMatrix;

MObject		baconDoubleBezier::parentInverseMatrix;
MObject		baconDoubleBezier::jointOrient;
MObject		baconDoubleBezier::jointOrientX;
MObject		baconDoubleBezier::jointOrientY;
MObject		baconDoubleBezier::jointOrientZ;
MObject		baconDoubleBezier::Rotation;
MObject		baconDoubleBezier::RotationX;
MObject		baconDoubleBezier::RotationY;
MObject		baconDoubleBezier::RotationZ;
MObject		baconDoubleBezier::Position;

MObject		baconDoubleBezier::spin;
MObject		baconDoubleBezier::spinStart;
MObject		baconDoubleBezier::spinEnd;



baconDoubleBezier::baconDoubleBezier() {}
baconDoubleBezier::~baconDoubleBezier() {}

MMatrix setRow( MMatrix matrix, MVector newVector, const int row)
{
	MMatrix returnTM = matrix;
	returnTM[row][0] = newVector[0];
	returnTM[row][1] = newVector[1];
	returnTM[row][2] = newVector[2];
	//MScriptUtil ArrayUtil = MScriptUtil();
	//ArrayUtil.setDoubleArray(matrix[row], 0, newVector[0]);
	//ArrayUtil.setDoubleArray(matrix[row], 1, newVector[1]);
	//ArrayUtil.setDoubleArray(matrix[row], 2, newVector[2]);
	return returnTM;
}

MMatrix transMatrix(MVector pos)
{
	MMatrix returnTM = setRow(MMatrix(), pos, 3);
	return returnTM;
}

MMatrix rotateXMatrix(float angle)
{
	MMatrix returnTM = MMatrix();
	float CAngle = cos(angle);
	float SAngle = sin(angle);
	returnTM = setRow(returnTM, MVector(1.0f, 0.0f, 0.0f), 0);
	returnTM = setRow(returnTM, MVector(0.0f, CAngle, SAngle), 1);
	returnTM = setRow(returnTM, MVector(0.0f, -1.0f * SAngle, CAngle), 2);
	returnTM = setRow(returnTM, MVector(0.0f, 0.0f, 0.0f), 3);
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

MVector Bezier4Interpolation(MVector P0, MVector P1, MVector P2, MVector P3, float u)
{
	float u2(u * u);
	float u3(u2 * u);
	return (P0 + (-P0 * 3.0f + u * (3.0f * P0 - P0 * u)) * u 
		+ (3.0f * P1 + u * (-6.0f * P1 + P1 * 3.0f * u)) * u 
		+ (P2 * 3.0f - P2 * 3.0f * u) * u2 + P3 * u3);
}


MVector Bezier4Tangent(MVector P0, MVector P1, MVector P2, MVector P3, float u)
{
	MVector C1(P3 - (3.0f * P2) + (3.0f * P1) - P0);
	MVector C2((3.0f * P2) - (6.0f * P1) + (3.0f * P0));
	MVector C3((3.0f * P1) - (3.0f * P0));
	return ((3.0f * C1 * u * u) + (2.0f * C2 * u) + C3);

}

MMatrix matrix3(MVector row1, MVector row2, MVector row3, MVector row4)
{
	MMatrix returnTM = MMatrix();
	returnTM = setRow(returnTM, row1, 0);
	returnTM = setRow(returnTM, row2, 1);
	returnTM = setRow(returnTM, row3, 2);
	returnTM = setRow(returnTM, row4, 3);
	return returnTM;
}


MStatus baconDoubleBezier::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if
	( 
		plug == Rotation		|| plug == Position			||
		plug == RotationX		|| plug == RotationY		|| plug == RotationZ
	)
	{
		// Handles and Values
		MDataHandle Curve1PercentHandle = data.inputValue(Curve1Percent, &returnStatus);
		float Curve1PercentValue(Curve1PercentHandle.asFloat());

		MDataHandle Curve2PercentHandle = data.inputValue(Curve2Percent, &returnStatus);
		float Curve2PercentValue(Curve2PercentHandle.asFloat());

		
		MDataHandle startCurve1WorldMatrixHandle = data.inputValue(startCurve1WorldMatrix, &returnStatus);
		MFloatMatrix s1FM(startCurve1WorldMatrixHandle.asFloatMatrix());
		MDataHandle endCurve1WorldMatrixHandle = data.inputValue(endCurve1WorldMatrix, &returnStatus);
		MFloatMatrix e1FM(endCurve1WorldMatrixHandle.asFloatMatrix());
		MDataHandle startCurve2WorldMatrixHandle = data.inputValue(startCurve2WorldMatrix, &returnStatus);
		MFloatMatrix s2FM(startCurve2WorldMatrixHandle.asFloatMatrix());
		MDataHandle endCurve2WorldMatrixHandle = data.inputValue(endCurve2WorldMatrix, &returnStatus);
		MFloatMatrix e2FM(endCurve2WorldMatrixHandle.asFloatMatrix());


		MDataHandle startTangent1WorldMatrixHandle = data.inputValue(startTangent1WorldMatrix, &returnStatus);
		MFloatMatrix st1FM(startTangent1WorldMatrixHandle.asFloatMatrix());
		MDataHandle endTangent1WorldMatrixHandle = data.inputValue(endTangent1WorldMatrix, &returnStatus);
		MFloatMatrix et1FM(endTangent1WorldMatrixHandle.asFloatMatrix());
		MDataHandle startTangent2WorldMatrixHandle = data.inputValue(startTangent2WorldMatrix, &returnStatus);
		MFloatMatrix st2FM(startTangent2WorldMatrixHandle.asFloatMatrix());
		MDataHandle endTangent2WorldMatrixHandle = data.inputValue(endTangent2WorldMatrix, &returnStatus);
		MFloatMatrix et2FM(endTangent2WorldMatrixHandle.asFloatMatrix());



		MDataHandle parentInverseMatrixHandle = data.inputValue(parentInverseMatrix, &returnStatus);
		MFloatMatrix pFM = parentInverseMatrixHandle.asFloatMatrix();

		MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();


		MDataHandle spinStartHandle = data.inputValue(spinStart, &returnStatus);
		MAngle spinStartValue = spinStartHandle.asAngle();
		MDataHandle spinEndHandle = data.inputValue(spinEnd, &returnStatus);
		MAngle spinEndValue = spinEndHandle.asAngle();



		/////////////////////////////////////////////////////////
		// Calculation
		MMatrix s1TM = FloatMatrixToMatrix(s1FM);
		MMatrix e1TM = FloatMatrixToMatrix(e1FM);
		MMatrix s2TM = FloatMatrixToMatrix(s2FM);
		MMatrix e2TM = FloatMatrixToMatrix(e2FM);

		MMatrix st1TM = FloatMatrixToMatrix(st1FM);
		MMatrix et1TM = FloatMatrixToMatrix(et1FM);
		MMatrix st2TM = FloatMatrixToMatrix(st2FM);
		MMatrix et2TM = FloatMatrixToMatrix(et2FM);

		MMatrix ipTM = FloatMatrixToMatrix(pFM);



		// Curve 1
		MVector c1P0(s1TM[3][0], s1TM[3][1], s1TM[3][2]);
		MVector c1P1(st1TM[3][0], st1TM[3][1], st1TM[3][2]);
		MVector c1P2(et1TM[3][0], et1TM[3][1], et1TM[3][2]);
		MVector c1P3(e1TM[3][0], e1TM[3][1], e1TM[3][2]);
		MVector Bezier1Position = Bezier4Interpolation(c1P0, c1P1, c1P2, c1P3, Curve1PercentValue);

		// Curve 2
		MVector c2P0(s2TM[3][0], s2TM[3][1], s2TM[3][2]);
		MVector c2P1(st2TM[3][0], st2TM[3][1], st2TM[3][2]);
		MVector c2P2(et2TM[3][0], et2TM[3][1], et2TM[3][2]);
		MVector c2P3(e2TM[3][0], e2TM[3][1], e2TM[3][2]);
		MVector Bezier2Position = Bezier4Interpolation(c2P0, c2P1, c2P2, c2P3, Curve2PercentValue);

		MVector Tangent1Axis = Bezier4Tangent(c2P0, c2P1, c2P2, c2P3, Curve2PercentValue);
		Tangent1Axis.normalize();

		MVector XAxis(Bezier2Position - Bezier1Position);
		XAxis.normalize();

		MVector YAxis = Tangent1Axis ^ XAxis;
		YAxis.normalize();
		MVector ZAxis = XAxis ^ YAxis;
		ZAxis.normalize();
		MMatrix finalWorldTM = matrix3(XAxis, YAxis, ZAxis, Bezier1Position);

		//Spin
		MAngle SpinBlend = ( spinStartValue.value() * (1.0 - Curve2PercentValue) ) + ( spinEndValue.value() * Curve2PercentValue );
		MMatrix SpinTM = MEulerRotation(SpinBlend.value(), 0.0, 0.0).asMatrix();


		// Finalize Local
		MMatrix localTM = SpinTM * finalWorldTM * ipTM;
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

			// RotationX
			MDataHandle RotationXHandle = data.outputValue(baconDoubleBezier::RotationX);
			RotationXHandle.setMAngle(MAngle(outputAngles.x));
			RotationXHandle.setClean();
			// RotationY
			MDataHandle RotationYHandle = data.outputValue(baconDoubleBezier::RotationY);
			RotationYHandle.setMAngle(MAngle(outputAngles.y));
			RotationYHandle.setClean();
			// RotationX
			MDataHandle RotationZHandle = data.outputValue(baconDoubleBezier::RotationZ);
			RotationZHandle.setMAngle(MAngle(outputAngles.z));
			RotationZHandle.setClean();

			//  Position
			MDataHandle PositionHandle = data.outputValue(baconDoubleBezier::Position);
			PositionHandle.set3Float(float(localTM[3][0]), float(localTM[3][1]), float(localTM[3][2]));
				//setMVector(MVector(outputTM[3][0], outputTM[3][1], outputTM[3][2]));
			PositionHandle.setClean();

		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconDoubleBezier::creator()
{
	return new baconDoubleBezier();
}


MStatus baconDoubleBezier::initialize()
{
	MFnCompoundAttribute	compAttr;
	MFnNumericAttribute numAttr;
	MFnUnitAttribute	uAttr;
	MFnMatrixAttribute	matrixAttr;
	MStatus				stat;

	// INPUTS ---------------------------------------------------------------------
	
	// Curve1Percent
	Curve1Percent = numAttr.create("Curve1Percent", "pp", MFnNumericData::kFloat, 0.0);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	stat = addAttribute(Curve1Percent);

	// Curve2Percent
	Curve2Percent = numAttr.create("Curve2Percent", "pl", MFnNumericData::kFloat, 0.25);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	stat = addAttribute(Curve2Percent);

	//------------------------------
	// spinStart
	spinStart = uAttr.create("spinStart", "sps", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// spinEnd
	spinEnd = uAttr.create("spinEnd", "spe", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setStorable(true);

	// Spin Compound
	spin = numAttr.create("spin", "sp", spinStart, spinEnd);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	stat = addAttribute(spin);




	//------------------------------
	// Compund Plug for Curve 1
	compCurve1 = compAttr.create("curve1Properties", "compc1");
	compAttr.setKeyable(true);
	compAttr.setWritable(true);

	// input startCurve1WorldMatrix
	startCurve1WorldMatrix = matrixAttr.create("startCurve1WorldMatrix", "s1TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	//stat = addAttribute(startCurve1WorldMatrix);
	compAttr.addChild(startCurve1WorldMatrix);

	// input startTangent1WorldMatrix
	startTangent1WorldMatrix = matrixAttr.create("startTangent1WorldMatrix", "st1TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	// = addAttribute(startTangent1WorldMatrix);
	compAttr.addChild(startTangent1WorldMatrix);

	// input endTangent1WorldMatrix
	endTangent1WorldMatrix = matrixAttr.create("endTangent1WorldMatrix", "et1TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	//stat = addAttribute(endTangent1WorldMatrix);
	compAttr.addChild(endTangent1WorldMatrix);

	// input endCurve1WorldMatrix
	endCurve1WorldMatrix = matrixAttr.create("endCurve1WorldMatrix", "e1TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	//stat = addAttribute(endCurve1WorldMatrix);
	compAttr.addChild(endCurve1WorldMatrix);
	stat = addAttribute(compCurve1);


	//------------------------------
	// Compund Plug for Curve 2
	compCurve2 = compAttr.create("curve2Properties", "compc2");
	compAttr.setKeyable(true);
	compAttr.setWritable(true);

	// input startCurve2WorldMatrix
	startCurve2WorldMatrix = matrixAttr.create("startCurve2WorldMatrix", "s2TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(startCurve2WorldMatrix);

	// input startTangent2WorldMatrix
	startTangent2WorldMatrix = matrixAttr.create("startTangent2WorldMatrix", "st2TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(startTangent2WorldMatrix);

	// input endTangent2WorldMatrix
	endTangent2WorldMatrix = matrixAttr.create("endTangent2WorldMatrix", "et2TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(endTangent2WorldMatrix);

	// input endCurve2WorldMatrix
	endCurve2WorldMatrix = matrixAttr.create("endCurve2WorldMatrix", "e2TM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(endCurve2WorldMatrix);
	stat = addAttribute(compCurve2);


	//------------------------------
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



	//------------------------------
	// input parentInverseMatrix
	parentInverseMatrix = matrixAttr.create("parentInverseMatrix", "piTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentInverseMatrix);


	// OUTUTS ---------------------------------------------------------------------

	// Output  rotationX
	RotationX = uAttr.create("RotationX", "arotX", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output  rotationY
	RotationY = uAttr.create("RotationY", "arotY", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output  rotationZ
	RotationZ = uAttr.create("RotationZ", "arotZ", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output  rotation
	Rotation = numAttr.create("Rotation", "arot", RotationX, RotationY, RotationZ);
	numAttr.setHidden(false);
	stat = addAttribute(Rotation);

	// Output  Position
	Position = numAttr.createPoint("Position", "apos");
	numAttr.setStorable(false);
	numAttr.setHidden(false);
	stat = addAttribute(Position);

	
	//AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] =
	{	Rotation,	Position,
		RotationX,	RotationY,	RotationZ
	};
	for (MObject& obj : AffectedByMany) 
	{
		attributeAffects(jointOrient,				obj);
		attributeAffects(jointOrientX,				obj);
		attributeAffects(jointOrientY,				obj);
		attributeAffects(jointOrientZ,				obj);
		attributeAffects(startCurve1WorldMatrix,	obj);
		attributeAffects(endCurve1WorldMatrix,		obj);
		attributeAffects(startCurve2WorldMatrix,	obj);
		attributeAffects(endCurve2WorldMatrix,		obj);
		attributeAffects(startTangent1WorldMatrix,  obj);
		attributeAffects(endTangent1WorldMatrix,    obj);
		attributeAffects(startTangent2WorldMatrix,  obj);
		attributeAffects(endTangent2WorldMatrix,    obj);
		attributeAffects(parentInverseMatrix,		obj);
		attributeAffects(Curve1Percent,				obj);
		attributeAffects(Curve2Percent,				obj);
		attributeAffects(spinStart,					obj);
		attributeAffects(spinEnd,					obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin(obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode( "baconDoubleBezier", baconDoubleBezier::id, baconDoubleBezier::creator,
								  baconDoubleBezier::initialize );
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

	status = plugin.deregisterNode( baconDoubleBezier::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
