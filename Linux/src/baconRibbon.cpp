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
// Produces the dependency graph node "baconRibbon".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID:
// 0x0012a955
//
////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <maya/MIOStream.h>
#include <string.h>
#include <array>

#include <maya/MPxNode.h>

#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnUnitAttribute.h>

#include <maya/MAngle.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MPlug.h>
#include <maya/MQuaternion.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MVector.h>

#include <maya/MDoubleArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPoint.h>
#include <maya/MScriptUtil.h>
#include <maya/MTransformationMatrix.h>


class baconRibbon : public MPxNode
{
   public:
	baconRibbon();
	virtual ~baconRibbon();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	static void* creator();
	static MStatus initialize();

   public:
	static MTypeId id;

	// component groups
	static MObject twistComp;
	static MObject BezierControllersComp;

	// inputs
	static MObject pathPercent;	 // The pathPercent value for the start.
	static MObject pathLength;
	static MObject twistStart;
	static MObject twistEnd;
	static MObject numberOfSegments;
	static MObject segment;

	static MObject startWorldMatrix;
	static MObject endWorldMatrix;
	static MObject startTangentWorldMatrix;
	static MObject endTangentWorldMatrix;
	static MObject upObjectWorldMatrix;

	static MObject separateTangents;
	static MObject rollType;
	static MObject pathLocationMethod;
	static MObject upVectorDirection;
	static MObject upVectorObject;

	static MObject parentInverseMatrix;
	static MObject jointOrient;
	static MObject jointOrientX;
	static MObject jointOrientY;
	static MObject jointOrientZ;

	// ouputs
	static MObject alignedRotation;
	static MObject alignedRotationX;
	static MObject alignedRotationY;
	static MObject alignedRotationZ;
	static MObject alignedPosition;
};

MTypeId baconRibbon::id(0x0012a955);
MObject baconRibbon::pathPercent;
MObject baconRibbon::pathLength;
MObject baconRibbon::twistComp;
MObject baconRibbon::BezierControllersComp;
MObject baconRibbon::twistStart;
MObject baconRibbon::twistEnd;
MObject baconRibbon::startWorldMatrix;
MObject baconRibbon::endWorldMatrix;
MObject baconRibbon::startTangentWorldMatrix;
MObject baconRibbon::endTangentWorldMatrix;
MObject baconRibbon::upObjectWorldMatrix;
MObject baconRibbon::parentInverseMatrix;
MObject baconRibbon::jointOrient;
MObject baconRibbon::jointOrientX;
MObject baconRibbon::jointOrientY;
MObject baconRibbon::jointOrientZ;
MObject baconRibbon::alignedRotation;
MObject baconRibbon::alignedRotationX;
MObject baconRibbon::alignedRotationY;
MObject baconRibbon::alignedRotationZ;
MObject baconRibbon::alignedPosition;
MObject baconRibbon::separateTangents;
MObject baconRibbon::rollType;
MObject baconRibbon::pathLocationMethod;
MObject baconRibbon::numberOfSegments;
MObject baconRibbon::segment;
MObject baconRibbon::upVectorObject;
MObject baconRibbon::upVectorDirection;

baconRibbon::baconRibbon() {}
baconRibbon::~baconRibbon() {}

MMatrix setRow(MMatrix matrix, MVector newVector, const int row)
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

MMatrix rotateXMatrix(float angle)
{
	MMatrix returnTM = MMatrix();
	const float CAngle = cos(angle);
	const float SAngle = sin(angle);
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
	float const u2(u * u);
	float const u3(u2 * u);
	return (P0 + (-P0 * 3.0f + u * (3.0f * P0 - P0 * u)) * u + (3.0f * P1 + u * (-6.0f * P1 + P1 * 3.0f * u)) * u + (P2 * 3.0f - P2 * 3.0f * u) * u2 + P3 * u3);
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

double quatDot(MQuaternion q1, MQuaternion q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

MQuaternion baconSlerp(MQuaternion a, MQuaternion b, double t)
{
	double omega = quatDot(a, b);
	if (omega < 0.0)
	{
		omega = quatDot(a, b.negateIt());
	}
	double theta = acos(omega);
	double epsilon = sin(theta);

	double w1(0.0);
	double w2(0.0);

	if (epsilon > 0.0001)
	{
		w1 = sin(theta * (1.0 - t)) / epsilon;
		w2 = sin(theta * t) / epsilon;
	}
	else
	{
		w1 = 1.0 - t;
		w2 = t;
	}

	MQuaternion aa(a.scaleIt(w1));
	MQuaternion bb(b.scaleIt(w2));
	return aa + bb;
}

MVector getRow(MMatrix TM, int row)
{
	return (MVector(TM[row][0], TM[row][1], TM[row][2]));
}

MStatus baconRibbon::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus returnStatus;

	if (plug == alignedRotation || plug == alignedPosition || plug == alignedRotationX || plug == alignedRotationY || plug == alignedRotationZ)
	{
		// Handles and Values
		const MDataHandle pathPercentHandle = data.inputValue(pathPercent, &returnStatus);
		float pathPercentValue(pathPercentHandle.asFloat());

		MDataHandle pathLocationMethodHandle = data.inputValue(pathLocationMethod, &returnStatus);
		int pathLocationMethodValue = pathLocationMethodHandle.asShort();

		MDataHandle numberOfSegmentsHandle = data.inputValue(numberOfSegments, &returnStatus);
		float numberOfSegmentsValue = float(numberOfSegmentsHandle.asShort());

		MDataHandle segmentHandle = data.inputValue(segment, &returnStatus);
		float segmentValue = float(segmentHandle.asShort());

		if (pathLocationMethodValue == 1)
		{
			float segementPercent(1.0f / numberOfSegmentsValue);
			pathPercentValue = (segementPercent * (segmentValue - 1.0f)) + (segementPercent / 2.0f);
		}



		MDataHandle separateTangentsHandle = data.inputValue(separateTangents, &returnStatus);
		bool separateTangentsValue = separateTangentsHandle.asBool();

		MDataHandle rollTypeHandle = data.inputValue(rollType, &returnStatus);
		int rollTypeValue = rollTypeHandle.asShort();

		MDataHandle upVectorDirectionHandle = data.inputValue(upVectorDirection, &returnStatus);
		int upVectorDirectionValue = upVectorDirectionHandle.asShort();

		MDataHandle upVectorObjectHandle = data.inputValue(upVectorObject, &returnStatus);
		int upVectorObjectValue = upVectorObjectHandle.asShort();

		const MDataHandle twistStartHandle = data.inputValue(twistStart, &returnStatus);
		MAngle twistStartValue(twistStartHandle.asAngle());

		const MDataHandle twistEndHandle = data.inputValue(twistEnd, &returnStatus);
		MAngle twistEndValue(twistEndHandle.asAngle());

		const MDataHandle startWorldMatrixHandle = data.inputValue(startWorldMatrix, &returnStatus);
		MFloatMatrix sFM(startWorldMatrixHandle.asFloatMatrix());

		const MDataHandle endWorldMatrixHandle = data.inputValue(endWorldMatrix, &returnStatus);
		MFloatMatrix eFM(endWorldMatrixHandle.asFloatMatrix());

		const MDataHandle startTangentWorldMatrixHandle = data.inputValue(startTangentWorldMatrix, &returnStatus);
		MFloatMatrix stFM(startTangentWorldMatrixHandle.asFloatMatrix());

		const MDataHandle endTangentWorldMatrixHandle = data.inputValue(endTangentWorldMatrix, &returnStatus);
		MFloatMatrix etFM(endTangentWorldMatrixHandle.asFloatMatrix());

		const MDataHandle upObjectWorldMatrixHandle = data.inputValue(upObjectWorldMatrix, &returnStatus);
		MFloatMatrix upFM(upObjectWorldMatrixHandle.asFloatMatrix());

		const MDataHandle parentInverseMatrixHandle = data.inputValue(parentInverseMatrix, &returnStatus);
		MFloatMatrix pFM = parentInverseMatrixHandle.asFloatMatrix();

		const MDataHandle jointOrientXHandle = data.inputValue(jointOrientX, &returnStatus);
		MAngle jointOrientXValue = jointOrientXHandle.asAngle();
		const MDataHandle jointOrientYHandle = data.inputValue(jointOrientY, &returnStatus);
		MAngle jointOrientYValue = jointOrientYHandle.asAngle();
		const MDataHandle jointOrientZHandle = data.inputValue(jointOrientZ, &returnStatus);
		MAngle jointOrientZValue = jointOrientZHandle.asAngle();

		/////////////////////////////////////////////////////////
		// Calculation
		// Get position and tangent at percent value.

		const MMatrix sTM = FloatMatrixToMatrix(sFM);
		const MMatrix eTM = FloatMatrixToMatrix(eFM);
		const MMatrix stTM = FloatMatrixToMatrix(stFM);
		const MMatrix etTM = FloatMatrixToMatrix(etFM);
		const MMatrix ipTM = FloatMatrixToMatrix(pFM);
		const MMatrix upTM = FloatMatrixToMatrix(upFM);

		MVector P0(sTM[3][0], sTM[3][1], sTM[3][2]);
		MVector P1(stTM[3][0], stTM[3][1], stTM[3][2]);
		MVector P2(etTM[3][0], etTM[3][1], etTM[3][2]);
		MVector P3(eTM[3][0], eTM[3][1], eTM[3][2]);
		MVector Pup(upTM[3][0], upTM[3][1], upTM[3][2]);

		if (separateTangentsValue == false)
		{
			MVector X0(sTM[0][0], sTM[0][1], sTM[0][2]);
			MVector X3(eTM[0][0], eTM[0][1], eTM[0][2]);
			P1 = P0 + X0;
			P2 = P3 + (X3 * -1.0);	// want to use -X if we want the controls to be in the same direction as bones.
		}

		// Up Vector TM
		MMatrix UpNodeTM;
		if (upVectorObjectValue == 0)
		{
			UpNodeTM = sTM;
		}
		else if (upVectorObjectValue == 1)
		{
			UpNodeTM = eTM;
		}
		else
		{
			UpNodeTM = upTM;
		}

		// Up Vector
		MVector UpVector;
		if (upVectorDirectionValue == 0)
		{
			UpVector = Pup - P0;
		}
		else if (upVectorDirectionValue == 1)
		{
			UpVector = MVector(UpNodeTM[0][0], UpNodeTM[0][1], UpNodeTM[0][2]);
		}
		else if (upVectorDirectionValue == 2)
		{
			UpVector = MVector(UpNodeTM[1][0], UpNodeTM[1][1], UpNodeTM[1][2]);
		}
		else if (upVectorDirectionValue == 3)
		{
			UpVector = MVector(UpNodeTM[2][0], UpNodeTM[2][1], UpNodeTM[2][2]);
		}
		else if (upVectorDirectionValue == 4)
		{
			UpVector = MVector(UpNodeTM[0][0], UpNodeTM[0][1], UpNodeTM[0][2]) * -1.0f;
		}
		else if (upVectorDirectionValue == 5)
		{
			UpVector = MVector(UpNodeTM[1][0], UpNodeTM[1][1], UpNodeTM[1][2]) * -1.0f;
		}
		else
		{
			UpVector = MVector(UpNodeTM[2][0], UpNodeTM[2][1], UpNodeTM[2][2]) * -1.0f;
		}

		UpVector.normalize();

		// Tangent Axis
		MVector BezierPosition = Bezier4Interpolation(P0, P1, P2, P3, pathPercentValue);
		MVector TangentAxis = Bezier4Tangent(P0, P1, P2, P3, pathPercentValue);
		TangentAxis.normalize();
		MVector YAxis = UpVector ^ TangentAxis;
		YAxis.normalize();
		MVector ZAxis = TangentAxis ^ YAxis;
		ZAxis.normalize();
		MMatrix TangentMatrix = matrix3(TangentAxis, YAxis, ZAxis, BezierPosition);

		// Get roll rotation
		MMatrix finalWorldTM;
		if (rollTypeValue == 0)	 // LERP
		{
			MQuaternion slerpRotation;
			if (separateTangentsValue == false)	 // Ends control rotation
			{
				MTransformationMatrix T0(sTM);
				MTransformationMatrix T3(eTM);
				slerpRotation = slerp(T0.rotation(), T3.rotation(), pathPercentValue);
			}
			else
			{
				// With Tangent Control
				MVector TangentAxis0(P1 - P0);
				TangentAxis0.normalize();
				MVector YAxis0 = UpVector ^ TangentAxis0;
				YAxis0.normalize();
				MVector ZAxis0 = TangentAxis0 ^ YAxis0;
				ZAxis0.normalize();
				MMatrix TMatrix0 = matrix3(TangentAxis0, YAxis0, ZAxis0, MVector(0, 0, 0));

				MVector TangentAxis3(P2 - P3);
				TangentAxis3.normalize();
				MVector YAxis3 = UpVector ^ TangentAxis3;
				YAxis3.normalize();
				MVector ZAxis3 = TangentAxis3 ^ YAxis3;
				ZAxis3.normalize();
				MMatrix TMatrix3 = matrix3(TangentAxis3, YAxis3, ZAxis3, MVector(0, 0, 0));

				MTransformationMatrix T0(TMatrix0);
				MTransformationMatrix T3(TMatrix3);
				slerpRotation = slerp(T0.rotation(), T3.rotation(), pathPercentValue);
			}

			// using baconRoll calculation for maximum accuracy.
			MMatrix targetRotationTM(slerpRotation.asMatrix());
			MMatrix localTTM(targetRotationTM * ipTM);
			MQuaternion localTRot = MTransformationMatrix(localTTM).rotation();
			MVector xAxis = getRow(localTTM, 0);
			xAxis.normalize();
			MVector vector = xAxis ^ MVector(1, 0, 0);
			vector.normalize();
			double angle = acos(xAxis.x);
			MQuaternion rollRot(angle, vector);
			rollRot = localTRot * rollRot;

			MTransformationMatrix rollTM(rollRot);
			finalWorldTM = rollTM.asMatrix() * TangentMatrix;
		}
		else
		{
			// TWIST Method
			double invPathPercentValue = 1.0 - pathPercentValue;
			double Twist = (twistStartValue.value() * invPathPercentValue) + (twistEndValue.value() * pathPercentValue);
			MMatrix twistMatrix = MEulerRotation(Twist, 0.0, 0.0).asMatrix();
			finalWorldTM = twistMatrix * TangentMatrix;
		}

		MMatrix localTM = finalWorldTM * ipTM;
		MMatrix jointOrientTM = MEulerRotation(jointOrientXValue.value(), jointOrientYValue.value(), jointOrientZValue.value()).asMatrix();
		MMatrix outputTM = localTM * jointOrientTM.inverse();
		MTransformationMatrix ouputTransformationMatrix = MTransformationMatrix(outputTM);
		MEulerRotation outputAngles = ouputTransformationMatrix.eulerRotation();

		// Set OutPut Values
		if (returnStatus != MS::kSuccess)
			cerr << "ERROR getting data" << endl;
		else
		{
			// alignedRotationX
			MDataHandle alignedRotationXHandle = data.outputValue(baconRibbon::alignedRotationX);
			alignedRotationXHandle.setMAngle(MAngle(outputAngles.x));
			alignedRotationXHandle.setClean();
			// alignedRotationY
			MDataHandle alignedRotationYHandle = data.outputValue(baconRibbon::alignedRotationY);
			alignedRotationYHandle.setMAngle(MAngle(outputAngles.y));
			alignedRotationYHandle.setClean();
			// alignedRotationX
			MDataHandle alignedRotationZHandle = data.outputValue(baconRibbon::alignedRotationZ);
			alignedRotationZHandle.setMAngle(MAngle(outputAngles.z));
			alignedRotationZHandle.setClean();

			// aligned Position
			MDataHandle alignedPositionHandle = data.outputValue(baconRibbon::alignedPosition);
			alignedPositionHandle.set3Float(localTM[3][0], localTM[3][1], localTM[3][2]);
			alignedPositionHandle.setClean();
		}
	}
	else
	{
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* baconRibbon::creator()
{
	return new baconRibbon();
}

MStatus baconRibbon::initialize()
{
	MFnCompoundAttribute compAttr;
	MFnNumericAttribute numAttr;
	MFnUnitAttribute uAttr;
	MFnMatrixAttribute matrixAttr;
	MFnEnumAttribute enumAttr;
	MStatus stat;

	// INPUTS ---------------------------------------------------------------------

	// pathLocationMethod
	pathLocationMethod = enumAttr.create("pathLocationMethod", "plm", 0);
	enumAttr.addField("Percentage", 0);
	enumAttr.addField("Segment Count", 1);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(pathLocationMethod);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}

	// pathPercent
	pathPercent = numAttr.create("pathPercent", "pp", MFnNumericData::kFloat, 0.0);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	stat = addAttribute(pathPercent);

	// pathLength 
	// This is an old method for tangent determination that is here for Maya
	// file compatibility for students with old scenes and "bendy bones".
	// It is set to hidden.
	pathLength = numAttr.create("pathLength", "pl", MFnNumericData::kFloat, 0.0);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	numAttr.setHidden(true);
	stat = addAttribute(pathLength);



	// segment
	segment = numAttr.create("segment", "seg", MFnNumericData::kShort, 1);
	numAttr.setMin(1);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	stat = addAttribute(segment);

	// numberOfSegments
	numberOfSegments = numAttr.create("numberOfSegments", "nseg", MFnNumericData::kShort, 1);
	numAttr.setMin(1);
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setWritable(true);
	stat = addAttribute(numberOfSegments);

	// separateTangents
	separateTangents = numAttr.create("separateTangents", "stan", MFnNumericData::kBoolean);
	numAttr.setDefault(true);
	numAttr.setKeyable(true);
	numAttr.setReadable(true);
	numAttr.setWritable(true);
	numAttr.setStorable(true);
	stat = addAttribute(separateTangents);

	// roll type
	rollType = enumAttr.create("rollType", "roll", 1);
	enumAttr.addField("LERP", 0);
	enumAttr.addField("Twist Values", 1);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(rollType);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}

	// upVectorObject
	upVectorObject = enumAttr.create("upVectorObject", "upObj", 0);
	enumAttr.addField("Start Controller", 0);
	enumAttr.addField("End Controller", 1);
	enumAttr.addField("Up Object", 2);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(upVectorObject);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}

	// upVectorDirection
	upVectorDirection = enumAttr.create("upVectorDirection", "updir", 3);
	enumAttr.addField("Pole Vector", 0);
	enumAttr.addField("X axis", 1);
	enumAttr.addField("Y axis", 2);
	enumAttr.addField("Z axis", 3);
	enumAttr.addField("-X axis", 4);
	enumAttr.addField("-Y axis", 5);
	enumAttr.addField("-Z axis", 6);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(upVectorDirection);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}


	// Twist value method
	twistComp = compAttr.create("twistValues", "twists");
	compAttr.setKeyable(true);
	compAttr.setWritable(true);

	twistStart = uAttr.create("twistStart", "ts", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setKeyable(true);
	compAttr.addChild(twistStart);

	twistEnd = uAttr.create("twistEnd", "te", uAttr.kAngle, 0.0);
	uAttr.setWritable(true);
	uAttr.setKeyable(true);
	compAttr.addChild(twistEnd);

	stat = addAttribute(twistComp);

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

	// input parentInverseMatrix
	parentInverseMatrix = matrixAttr.create("parentInverseMatrix", "piTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentInverseMatrix);


	// Bezier Controller Comp
	BezierControllersComp = compAttr.create("bezierControls", "bezierCont");
	compAttr.setKeyable(true);
	compAttr.setWritable(true);

	// input startWorldMatrix
	startWorldMatrix = matrixAttr.create("startWorldMatrix", "sTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(startWorldMatrix);

	// input startTangentWorldMatrix
	startTangentWorldMatrix = matrixAttr.create("startTangentWorldMatrix", "stTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(startTangentWorldMatrix);

	// input endWorldMatrix
	endWorldMatrix = matrixAttr.create("endWorldMatrix", "eTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(endWorldMatrix);

	// input endTangentWorldMatrix
	endTangentWorldMatrix = matrixAttr.create("endTangentWorldMatrix", "etTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(endTangentWorldMatrix);

		// input upObjectWorldMatrix
	upObjectWorldMatrix = matrixAttr.create("upObjectWorldMatrix", "upTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	compAttr.addChild(upObjectWorldMatrix);

	stat = addAttribute(BezierControllersComp);

	// OUTUTS ---------------------------------------------------------------------

	// Output aligned rotationX
	alignedRotationX = uAttr.create("rotationX", "arotx", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output aligned rotationY
	alignedRotationY = uAttr.create("rotationY", "aroty", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output aligned rotationZ
	alignedRotationZ = uAttr.create("rotationZ", "arotz", uAttr.kAngle, 0.0);
	uAttr.setWritable(false);
	// Output aligned rotation
	alignedRotation = numAttr.create("rotation", "arot", alignedRotationX, alignedRotationY, alignedRotationZ);
	numAttr.setHidden(false);
	stat = addAttribute(alignedRotation);

	// Output Aligned Position
	alignedPosition = numAttr.createPoint("position", "apos");
	numAttr.setStorable(false);
	numAttr.setHidden(false);
	stat = addAttribute(alignedPosition);

	// AFFECTS ---------------------------------------------------------------------
	MObject AffectedByMany[] = {alignedRotation, alignedPosition, alignedRotationX, alignedRotationY, alignedRotationZ};
	for (MObject& obj : AffectedByMany)
	{
		attributeAffects(jointOrient, obj);
		attributeAffects(jointOrientX, obj);
		attributeAffects(jointOrientY, obj);
		attributeAffects(jointOrientZ, obj);
		attributeAffects(startWorldMatrix, obj);
		attributeAffects(endWorldMatrix, obj);
		attributeAffects(startTangentWorldMatrix, obj);
		attributeAffects(endTangentWorldMatrix, obj);
		attributeAffects(upObjectWorldMatrix, obj);
		attributeAffects(parentInverseMatrix, obj);
		attributeAffects(pathPercent, obj);
		attributeAffects(twistStart, obj);
		attributeAffects(twistEnd, obj);
		attributeAffects(rollType, obj);
		attributeAffects(pathLocationMethod, obj);
		attributeAffects(separateTangents, obj);
		attributeAffects(upVectorDirection, obj);
		attributeAffects(upVectorObject, obj);
		attributeAffects(numberOfSegments, obj);
		attributeAffects(segment, obj);
	}

	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode("baconRibbon", baconRibbon::id, baconRibbon::creator, baconRibbon::initialize);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(baconRibbon::id);
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
