
#ifndef __baconMath_h 
#define __baconMath_h 

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
#include <maya/MFnTransform.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MAngle.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFloatPoint.h>
#include <maya/MQuaternion.h>
#include <maya/MFloatMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MScriptUtil.h>

MMatrix setRow( MMatrix matrix, MVector newVector, const int row)
{
	MMatrix returnTM = matrix;
	returnTM[row][0] = newVector[0];
	returnTM[row][1] = newVector[1];
	returnTM[row][2] = newVector[2];
	return returnTM;
}

MVector getRow(MMatrix matrix, int row)
{
	return ( MVector(matrix[row][0], matrix[row][1], matrix[row][2]) );
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

static MFloatMatrix MMatrixToFloatMatrix(MMatrix MM)
{
	MFloatMatrix returnTM = MFloatMatrix();
	returnTM[0][0] = float(MM[0][0]);
	returnTM[0][1] = float(MM[0][1]);
	returnTM[0][2] = float(MM[0][2]);
	returnTM[0][3] = float(MM[0][3]);
	returnTM[1][0] = float(MM[1][0]);
	returnTM[1][1] = float(MM[1][1]);
	returnTM[1][2] = float(MM[1][2]);
	returnTM[1][3] = float(MM[1][3]);
	returnTM[2][0] = float(MM[2][0]);
	returnTM[2][1] = float(MM[2][1]);
	returnTM[2][2] = float(MM[2][2]);
	returnTM[2][3] = float(MM[2][3]);
	returnTM[3][0] = float(MM[3][0]);
	returnTM[3][1] = float(MM[3][1]);
	returnTM[3][2] = float(MM[3][2]);
	returnTM[3][3] = float(MM[3][3]);
	return returnTM;
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


static MMatrix scaleMatrix(MVector sca)
{
	MMatrix returnTM = MMatrix();
	returnTM = setRow(returnTM, MVector(sca[0], 0.0, 0.0), 0);
	returnTM = setRow(returnTM, MVector(0.0, sca[1], 0.0), 1);
	returnTM = setRow(returnTM, MVector(0.0, 0.0, sca[2]), 2);
	returnTM = setRow(returnTM, MVector(0.0, 0.0, 0.0), 3);
	return returnTM;
}

MMatrix mirrorMatrix(MMatrix inTM)
{
	MMatrix returnTM = MMatrix();
	returnTM = setRow(returnTM, (MVector(inTM[0][0] * -1.0, inTM[0][1] * -1.0, inTM[0][2] * -1.0)), 0);
	returnTM = setRow(returnTM, (MVector(inTM[1][0] * -1.0, inTM[1][1] * -1.0, inTM[1][2] * -1.0)), 1);
	returnTM = setRow(returnTM, (MVector(inTM[2][0] * -1.0, inTM[2][1] * -1.0, inTM[2][2] * -1.0)), 2);
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


bool eulerValueCheck(MEulerRotation &inRot )
{
	if (isnan(inRot.x)) { inRot.x = 0.0; }
	if (isnan(inRot.y)) { inRot.y = 0.0; }
	if (isnan(inRot.z)) { inRot.z = 0.0; }
	return true;
}

MAngle cosineLaw(float A, float B, float C)
{
	float A2(pow(A, 2));
	float B2(pow(B, 2));
	float C2(pow(C, 2));
	float AB(2.0f * A * B);
	MAngle angle(0);
	try
	{
		angle = acos((A2 + B2 - C2) / AB);
	}
	catch (...)
	{
		angle = MAngle(0);
	}
	return angle;
}


double quatDot(MQuaternion q1, MQuaternion q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
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


MQuaternion MSlerp(MQuaternion qa, MQuaternion qb, double t) 
{
	// quaternion to return
	MQuaternion qm(MQuaternion::identity);

	double cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	if (abs(cosHalfTheta) >= 1.0) {
		qm.w = qa.w; qm.x = qa.x; qm.y = qa.y; qm.z = qa.z;
		return qm;
	}
	// Calculate temporary values.
	double halfTheta = acos(cosHalfTheta);
	double sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
	if (abs(sinHalfTheta) < 0.001) { 
		qm.w = (qa.w * 0.5 + qb.w * 0.5);
		qm.x = (qa.x * 0.5 + qb.x * 0.5);
		qm.y = (qa.y * 0.5 + qb.y * 0.5);
		qm.z = (qa.z * 0.5 + qb.z * 0.5);
		return qm;
	}
	double ratioA = sin((1.0 - t) * halfTheta) / sinHalfTheta;
	double ratioB = sin(t * halfTheta) / sinHalfTheta;

	//calculate Quaternion.
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.x = (qa.x * ratioA + qb.x * ratioB);
	qm.y = (qa.y * ratioA + qb.y * ratioB);
	qm.z = (qa.z * ratioA + qb.z * ratioB);
	return qm;
}

#endif