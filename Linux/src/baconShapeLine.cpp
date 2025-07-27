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
// Produces the dependency graph node "baconShapeLine".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a941
//
////////////////////////////////////////////////////////////////////////


#include <maya/MCallbackIdArray.h>
#include <maya/MDagMessage.h>
#include <maya/MDrawRegistry.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFrameContext.h>
#include <maya/MItDag.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFloatMatrix.h>
#include <maya/MTransformationMatrix.h>


#include <stdio.h>

#include "baconMath.h"

using namespace MHWRender;


class baconShapeLine : public MPxLocatorNode
{
public:


	enum ELineType
	{
		kSolid,
		kDotted,
		kDashed,
		kShortDotted,
		kShortDashed
	};

	static void* creator() { return new baconShapeLine(); }

	static MStatus initialize();

	// Registration
	static char*   className;
	static MTypeId id;
	static MString drawDbClassification;
	static MString drawRegistrantId;

	// Attributes
	static MObject aTextColor;
	static MObject aLineType;
	static MObject targetMatrix;
	static MObject parentTM;
	static MObject wireIntensity;

private:

	/*
	baconShapeLine();
	*/

	virtual ~baconShapeLine();

	/* 
	// REMOVING CALLBACKS SINCE IT'S FASTER TO SIMPLY DRAW THE LINE
	// This plugin is based on "transformDrawNode" which is designed to 
	// draw lines on all meshes in the scene. 
	static void AllDagChangesCallback(
		MDagMessage::DagMessage msgType,
		MDagPath &child,
		MDagPath &parent,
		void *clientData);

	static void WorldMatrixModifiedCallback(
		MObject &transformNode,
		MDagMessage::MatrixModifiedFlags &modified,
		void *clientData);

	void processDagMessage(bool refreshWorldMatrixCbIds);

	MCallbackId fAllDagChangesCbId;
	MCallbackIdArray fWorldMatrixModifiedCbIds;
	*/

};

// Registration
char*   baconShapeLine::className = "baconShapeLine";
MTypeId baconShapeLine::id(0x0012a941);
MString baconShapeLine::drawDbClassification("drawdb/geometry/baconShapeLine");
MString baconShapeLine::drawRegistrantId(baconShapeLine::className);

// Attributes
MObject baconShapeLine::aTextColor;
MObject baconShapeLine::aLineType;
MObject	baconShapeLine::targetMatrix;
MObject	baconShapeLine::parentTM;
MObject	baconShapeLine::wireIntensity;

MStatus baconShapeLine::initialize()
{

	MStatus				stat;

	MFnNumericAttribute nAttr;
	aTextColor = nAttr.create("wireColor", "tc", MFnNumericData::k3Float);
	nAttr.setDefault(1.0f, 1.0f, 1.0f);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setReadable(true);
	nAttr.setUsedAsColor(true);
	MPxNode::addAttribute(aTextColor);

	MFnEnumAttribute eAttr;
	aLineType = eAttr.create("lineType", "tt", kDotted);
	eAttr.addField("Solid Line", kSolid);
	eAttr.addField("Dots", kDotted);
	eAttr.addField("Dashes", kDashed);
	eAttr.addField("Spaced Dots", kShortDotted);
	eAttr.addField("Spaced Dashes", kShortDashed);
	eAttr.setStorable(true);
	eAttr.setKeyable(true);
	eAttr.setHidden(false);
	eAttr.setWritable(true);
	eAttr.setReadable(true);
	MPxNode::addAttribute(aLineType);

	MFnNumericAttribute wAttr;
	wireIntensity = wAttr.create("wireIntensity", "wi", MFnNumericData::kDouble);
	wAttr.setDefault(0.3);
	wAttr.setStorable(true);
	wAttr.setKeyable(true);
	wAttr.setHidden(false);
	wAttr.setWritable(true);
	wAttr.setReadable(true);
	MPxNode::addAttribute(wireIntensity);


	MFnMatrixAttribute	matrixAttr;
	// input targetMatrix
	targetMatrix = matrixAttr.create("targetMatrix", "twTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(targetMatrix);

	// input parentTM
	parentTM = matrixAttr.create("parentTM", "pTM", matrixAttr.kFloat);
	matrixAttr.setStorable(true);
	matrixAttr.setKeyable(true);
	stat = addAttribute(parentTM);


	return MS::kSuccess;
}


/*
baconShapeLine::baconShapeLine() : MPxLocatorNode()
{
	fAllDagChangesCbId = MDagMessage::addAllDagChangesCallback(
		AllDagChangesCallback, this);
}
*/

baconShapeLine::~baconShapeLine()
{
	/*
	if (fWorldMatrixModifiedCbIds.length() > 0)
	{
		MMessage::removeCallbacks(fWorldMatrixModifiedCbIds);
	}

	if (fAllDagChangesCbId != 0)
	{
		MMessage::removeCallback(fAllDagChangesCbId);
	}
	*/
}


/*
void baconShapeLine::processDagMessage(bool refreshWorldMatrixModifiedCbIds)
{
	// Explicitly mark the node as being dirty on certain DAG message callbacks
	// so that the draw override can be updated.
	MRenderer::setGeometryDrawDirty(thisMObject());

	if (refreshWorldMatrixModifiedCbIds)
	{
		MStatus status;
		MItDag dagIt(MItDag::kDepthFirst, MFn::kMesh, &status);
		if (status)
		{
			// Remove existing callbacks
			if (fWorldMatrixModifiedCbIds.length() > 0)
			{
				MMessage::removeCallbacks(fWorldMatrixModifiedCbIds);
				fWorldMatrixModifiedCbIds.clear();
			}

			// Add new callbacks
			for (; !dagIt.isDone(); dagIt.next())
			{
				MDagPath dagPath;
				status = dagIt.getPath(dagPath);
				if (status)
				{
					MCallbackId id = MDagMessage::addWorldMatrixModifiedCallback(
						dagPath, WorldMatrixModifiedCallback, this);
					fWorldMatrixModifiedCbIds.append(id);
				}
			}
		}
	}
}

void baconShapeLine::AllDagChangesCallback(
	MDagMessage::DagMessage msgType,
	MDagPath &child,
	MDagPath &parent,
	void *clientData)
{
	// We need to refresh the world matrix modified callbacks because the DAG
	// is just changed.
	baconShapeLine *node = static_cast<baconShapeLine*>(clientData);
	if (node) node->processDagMessage(true);
}

void baconShapeLine::WorldMatrixModifiedCallback(
	MObject &transformNode,
	MDagMessage::MatrixModifiedFlags &modified,
	void *clientData)
{
	// We don't need to refresh the world matrix modified callbacks because the
	// DAG is not changed.

	baconShapeLine *node = static_cast<baconShapeLine*>(clientData);
	if (node) node->processDagMessage(false);

}

*/

class transformDrawData : public MUserData
{
public:
	transformDrawData()
		: MUserData(false)
		, fTextColor(1.0f, 1.0f, 1.0f, 1.0f)
		, fWireIntensity(0.4)
		, fLineType(baconShapeLine::kDotted)
	{
	}

	virtual ~transformDrawData() {}

	MColor fTextColor;
	double fWireIntensity;
	baconShapeLine::ELineType fLineType;

	//MVectorArray fPositions;
	//MVectorArray fVectors;
	MVector fTargetPos;
};

class transformDrawOverride : public MPxDrawOverride
{
public:

	static MPxDrawOverride* creator(const MObject& obj)
	{
		return new transformDrawOverride(obj);
	}

	virtual DrawAPI supportedDrawAPIs() const { return kAllDevices; }

	virtual bool hasUIDrawables() const { return true; }

	virtual MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MFrameContext& frameContext,
		MUserData* oldData);

	virtual void addUIDrawables(
		const MDagPath& objPath,
		MUIDrawManager& drawManager,
		const MFrameContext& frameContext,
		const MUserData* data);

private:

	// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
	// draw override will be updated (via prepareForDraw()) only when the node
	// is marked dirty via DG evaluation or dirty propagation. Additional
	// callbacks are also added to explicitly mark the node as being dirty (via
	// MRenderer::setGeometryDrawDirty()) for certain circumstances. Note that
	// the draw callback in MPxDrawOverride constructor is set to NULL in order
	// to achieve better performance.

	// "isAlwaysDirty" SET TO TRUE
	// This way it gets flagged as dirty when lookat objects transform. 
	// This is faster than callbacks since you have to navigate through dagpaths
	// if not.

	transformDrawOverride(const MObject& obj) : MPxDrawOverride(obj, NULL, true) {}
	virtual ~transformDrawOverride() {}
};


MUserData* transformDrawOverride::prepareForDraw(
	const MDagPath& objPath,
	const MDagPath& cameraPath,
	const MFrameContext& frameContext,
	MUserData* oldData)
{
	MStatus status;
	MObject obj = objPath.node(&status);
	if (!status) return NULL;

	transformDrawData* tdData = dynamic_cast<transformDrawData*>(oldData);
	if (!tdData)
	{
		tdData = new transformDrawData();
	}

	/*
	MStatus returnStatus;
	MDataHandle worldTMhandle = data.inputValue(baconShapeLine::worldMatrix, &returnStatus);
	MFloatMatrix worldTMValue = worldTMhandle.asFloatMatrix();
	*/

	//MPlug plugTM(obj, baconShapeLine::parentInverseMatrix);
	MPlug plugTM(obj, baconShapeLine::parentTM);
	MDataHandle worldTMhandle = plugTM.asMDataHandle();
	MFloatMatrix worldTMValue = worldTMhandle.asFloatMatrix();
	MMatrix thisTM = FloatMatrixToMatrix(worldTMValue);

	MPlug plugtTM(obj, baconShapeLine::targetMatrix);
	MDataHandle worldtTMhandle = plugtTM.asMDataHandle();
	MFloatMatrix worldtTMValue = worldtTMhandle.asFloatMatrix();
	MMatrix targetTM = FloatMatrixToMatrix(worldtTMValue);
	MTransformationMatrix localTM = targetTM * thisTM.inverse();
	MVector vec = localTM.getTranslation(MSpace::kWorld);
	tdData->fTargetPos = vec;


	//MObject oTM = plugTM.asMObject();
	//MFnMatrixAttribute nTM(oTM);

	//pluglocalScaleX.getValue(localScaleValX);


	// Text color
	{
		MPlug plug(obj, baconShapeLine::aTextColor);
		MObject o = plug.asMObject();
		MFnNumericData nData(o);
		nData.getData(tdData->fTextColor.r, tdData->fTextColor.g, tdData->fTextColor.b);

		MPlug plugIntensity(obj, baconShapeLine::wireIntensity);
		MDataHandle wireIntensityHandle = plugIntensity.asMDataHandle();
		tdData->fWireIntensity = wireIntensityHandle.asDouble();

	}

	// Type of Line
	MPlug plugLineType(obj, baconShapeLine::aLineType);
	tdData->fLineType = (baconShapeLine::ELineType)plugLineType.asInt();



	// Transform type
	/*
	{
	MPlug plug(obj, baconShapeLine::aLineType);
	tdData->fLineType = (baconShapeLine::ELineType)plug.asInt();
	}

	tdData->fPositions.clear();
	tdData->fVectors.clear();

	MItDag dagIt(MItDag::kDepthFirst, MFn::kMesh, &status);
	if (status)
	{
	for ( ; !dagIt.isDone(); dagIt.next() )
	{
	MDagPath dagPath;
	status = dagIt.getPath(dagPath);
	if (!status)
	{
	status.perror("MItDag::getPath");
	continue;
	}

	MObject transformNode = dagPath.transform(&status);
	if (!status)
	{
	status.perror("MDagPath::transform");
	continue;
	}

	MFnDagNode transform(transformNode, &status);
	if (!status)
	{
	status.perror("MFnDagNode constructor");
	continue;
	}

	//MTransformationMatrix matrix(transform.transformationMatrix());
	MTransformationMatrix localTM = targetTM * thisTM.inverse();
	MVector vec = localTM.getTranslation(MSpace::kWorld);
	//MVector vec = matrix.getTranslation(MSpace::kWorld);

	//vec = vec * thisTM.inverse();
	tdData->fPositions.append(vec);

	double tmp[3];
	MTransformationMatrix::RotationOrder order;
	switch (tdData->fLineType)
	{
	case baconShapeLine::kRotate:
	matrix.getRotation(tmp, order);
	vec = MVector(tmp);
	break;
	case baconShapeLine::kScale:
	matrix.getScale(tmp, MSpace::kWorld);
	vec = MVector(tmp);
	break;
	case baconShapeLine::kShear:
	matrix.getShear(tmp, MSpace::kWorld);
	vec = MVector(tmp);
	break;
	default:
	// Don't reset vec so that translation is drawn by default.
	break;
	}

	tdData->fVectors.append(vec);
	}
	}
	*/

	return tdData;
}

void transformDrawOverride::addUIDrawables(
	const MDagPath& objPath,
	MUIDrawManager& drawManager,
	const MFrameContext& frameContext,
	const MUserData* data)
{
	const transformDrawData* tdData = dynamic_cast<const transformDrawData*>(data);
	if (!tdData) return;

	drawManager.beginDrawable();
	MColor thisColor(tdData->fTextColor);
	//MUIDrawManager thisLineType(tdData->fLineType);

	drawManager.setColor(thisColor);

	// lines
	thisColor.a = float(tdData->fWireIntensity); //0.4;
	drawManager.setColor(thisColor);
	MPointArray singleLine;
	singleLine.clear();
	singleLine.append(0, 0, 0);
	singleLine.append(tdData->fTargetPos);

	//drawManager.setLineStyle(MHWRender::MUIDrawManager::kDotted);
	switch (tdData->fLineType)
	{
	case 1:
		drawManager.setLineStyle(MHWRender::MUIDrawManager::kDotted);
		break;
	case 2:
		drawManager.setLineStyle(MHWRender::MUIDrawManager::kDashed);
		break;
	case 3:
		drawManager.setLineStyle(MHWRender::MUIDrawManager::kShortDotted);
		break;
	case 4:
		drawManager.setLineStyle(MHWRender::MUIDrawManager::kShortDashed);
		break;
	default:
		drawManager.setLineStyle(MHWRender::MUIDrawManager::kSolid);
		break;
	}
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, singleLine);


	/*
	for (unsigned int i = 0; i < tdData->fVectors.length(); i++)
	{

	// text

	thisColor.a = 1.0;
	drawManager.setColor(thisColor);
	MPoint pos(tdData->fPositions[i]);
	MVector vec(tdData->fVectors[i]);
	char tmpStr[128] = {0};
	sprintf(tmpStr, "(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
	MString text(tmpStr);
	drawManager.text(pos, text, MUIDrawManager::kCenter);


	// lines
	thisColor.a = 0.1;
	drawManager.setColor(thisColor);
	MPointArray singleLine;
	singleLine.clear();
	singleLine.append(0, 0, 0);
	singleLine.append(pos);
	drawManager.setLineStyle(MHWRender::MUIDrawManager::kShortDotted);
	drawManager.mesh(MHWRender::MUIDrawManager::kLines, singleLine);


	}
	*/

	drawManager.endDrawable();
}


MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "Bacon-Strip.com", "1.0", "Any");

	MStatus status = plugin.registerNode(
		baconShapeLine::className,
		baconShapeLine::id,
		baconShapeLine::creator,
		baconShapeLine::initialize,
		MPxNode::kLocatorNode,
		&baconShapeLine::drawDbClassification);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	status = MDrawRegistry::registerDrawOverrideCreator(
		baconShapeLine::drawDbClassification,
		baconShapeLine::drawRegistrantId,
		transformDrawOverride::creator);
	if (!status)
	{
		status.perror("registerDrawOverrideCreator");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MStatus status = MDrawRegistry::deregisterDrawOverrideCreator(
		baconShapeLine::drawDbClassification,
		baconShapeLine::drawRegistrantId);
	if (!status)
	{
		status.perror("deregisterDrawOverrideCreator");
		return status;
	}

	status = plugin.deregisterNode(baconShapeLine::id);
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
