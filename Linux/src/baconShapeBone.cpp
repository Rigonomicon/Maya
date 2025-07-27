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
// Produces the dependency graph node "baconShapeBone".
// This plug-in is part of a rigging package known as The "Bacon-Strip Rig"
// Bacon-Strip ID Block is : 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip[Luis Alonso]
//
// ID: 
// 0x0012a940
//
////////////////////////////////////////////////////////////////////////


#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MPxCommand.h>
#include <maya/MTime.h>

// Parallel
#include <maya/MEvaluationNode.h>
#include <maya/MEvaluationManager.h>


// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>
#include <maya/MEventMessage.h>
#include <maya/MFnDependencyNode.h>

#include <assert.h>
#include "baconShapeBoneVisuals.h"



using namespace MHWRender;

static bool sUseLegacyDraw = (getenv("MAYA_ENABLE_VP2_PLUGIN_LOCATOR_LEGACY_DRAW") != NULL);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Node implementation with standard viewport draw
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class baconShapeBone : public MPxLocatorNode
{
public:
	baconShapeBone();
	virtual ~baconShapeBone();

	virtual MStatus   		compute(const MPlug& plug, MDataBlock& data);

/*
	virtual void            draw(M3dView & view, const MDagPath & path,
		M3dView::DisplayStyle style,
		M3dView::DisplayStatus status);
*/
	//virtual MPxNode::SchedulingType schedulingType(const MPxNode* update);

	
	// PARALLEL BS
	virtual MStatus         postEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType);
	// time input
	static  MObject     aTimeInput;
	// copies
	static MObject      aCopies;
	double scaleXBy;    // Scale value
	bool scaleUpToDate; // Is scale up to date


	//virtual bool            isBounded() const;
	//virtual MBoundingBox    boundingBox() const;
	//virtual bool			isTransparent() const;
	//virtual bool			drawLast() const;

	static  void *          creator();
	static  MStatus         initialize();

	static  MObject			aEnableTransparencySort;
	static  MObject			aEnableDrawLast;
	static  MObject			aTheColor;
	static  MObject			aTransparency;

	static  MObject         size;         
	static  MObject         xRay;
	static  MObject         mirror;

	static  MString         labelName;    
	static  MObject         wireVisibility;
	static  MObject         wireThickness;
	static  MObject         solidVisibility;

public:
	// Calculation for slowing down Maya and showing how postEvaluation works
	double doExpensiveCalculation(int c, double t);
	static	MTypeId		id;
	static	MString		drawDbClassification;
	static	MString		drawRegistrantId;
	static  MObject		shapeType;
	static  MObject		marionetteType;
	static  MObject		pickerType;
	static  MObject		boneType;
	static  MObject		prestoMode;

	static  MObject		localRotationX;
	static  MObject		localRotationY;
	static  MObject		localRotationZ;

	static  MObject		shapeOffset;
	static  MObject		shapeOffsetX;
	static  MObject		shapeOffsetY;
	static  MObject		shapeOffsetZ;
	static  MObject		shapeOrientation;
	static  MObject		shapeOrientationX;
	static  MObject		shapeOrientationY;
	static  MObject		shapeOrientationZ;
	static  MObject		shapeSize;
	static  MObject		shapeSizeX;
	static  MObject		shapeSizeY;
	static  MObject		shapeSizeZ;


};

MObject baconShapeBone::size;
MObject baconShapeBone::xRay;
MObject baconShapeBone::mirror;
MObject baconShapeBone::wireVisibility;
MObject baconShapeBone::wireThickness;
MObject baconShapeBone::solidVisibility;
MTypeId baconShapeBone::id(0x0012a940);
MObject baconShapeBone::aEnableTransparencySort;
MObject baconShapeBone::aEnableDrawLast;
MObject baconShapeBone::aTheColor;
MObject baconShapeBone::aTransparency;
MString	baconShapeBone::drawDbClassification("drawdb/geometry/baconShapeBone");
MString	baconShapeBone::drawRegistrantId("baconShapeBoneNodePlugin");
MObject	baconShapeBone::shapeType;
MObject	baconShapeBone::marionetteType;
MObject	baconShapeBone::pickerType;
MObject	baconShapeBone::boneType;
MObject baconShapeBone::prestoMode;

MObject	baconShapeBone::localRotationX;
MObject	baconShapeBone::localRotationY;
MObject	baconShapeBone::localRotationZ;

MObject	baconShapeBone::shapeOrientation;
MObject	baconShapeBone::shapeOrientationX;
MObject	baconShapeBone::shapeOrientationY;
MObject	baconShapeBone::shapeOrientationZ;
MObject	baconShapeBone::shapeOffset;
MObject	baconShapeBone::shapeOffsetX;
MObject	baconShapeBone::shapeOffsetY;
MObject	baconShapeBone::shapeOffsetZ;
MObject	baconShapeBone::shapeSize;
MObject	baconShapeBone::shapeSizeX;
MObject	baconShapeBone::shapeSizeY;
MObject	baconShapeBone::shapeSizeZ;



MObject baconShapeBone::aTimeInput;
MObject baconShapeBone::aCopies;

baconShapeBone::baconShapeBone()
	: scaleXBy(1.0)
	, scaleUpToDate(false)
{}
baconShapeBone::~baconShapeBone() {}

MStatus baconShapeBone::compute(const MPlug& /*plug*/, MDataBlock& /*data*/)
{
	return MS::kUnknownParameter;
}





void* baconShapeBone::creator()
{
	return new baconShapeBone();
}


/*

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Legacy Viewport 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void baconShapeBone::draw(M3dView & view, const MDagPath &,
	M3dView::DisplayStyle style,
	M3dView::DisplayStatus status)
{
	MObject thisNode = thisMObject();

	// Get shapeType
	//
	MPlug shapeTypePlug(thisNode, shapeType);
	int shapeTypeVal;
	shapeTypePlug.getValue(shapeTypeVal);

	// Get marionetteType
	//
	MPlug marionetteTypePlug(thisNode, marionetteType);
	int marionetteTypeVal;
	marionetteTypePlug.getValue(marionetteTypeVal);

	// Get the size
	//
	MPlug plug(thisNode, size);
	MDistance sizeVal;
	plug.getValue(sizeVal);
	float multiplier = (float)sizeVal.asCentimeters();


	// Get Mirror value
	//
	MPlug plugLocalMirror(thisNode, mirror);
	bool LocalMirrorVal;
	float mirrorSign;
	plugLocalMirror.getValue(LocalMirrorVal);
	if (LocalMirrorVal == true)
	{
		mirrorSign = -1.0;
	}
	else
	{
		mirrorSign = 1.0;
	}


	// Get the local position
	//
	MPlug pluglocalPositionX(thisNode, shapeOffsetX);
	MDistance localPositionValX;
	pluglocalPositionX.getValue(localPositionValX);

	MPlug pluglocalPositionY(thisNode, shapeOffsetY);
	MDistance localPositionValY;
	pluglocalPositionY.getValue(localPositionValY);

	MPlug pluglocalPositionZ(thisNode, shapeOffsetZ);
	MDistance localPositionValZ;
	pluglocalPositionZ.getValue(localPositionValZ);
	MVector localPosition
	(
		localPositionValX.asCentimeters() * mirrorSign,
		localPositionValY.asCentimeters() * mirrorSign,
		localPositionValZ.asCentimeters() * mirrorSign
	);

	// Get the local scale
	//


	MPlug pluglocalScaleX(thisNode, shapeSizeX);
	MDistance localScaleValX;
	pluglocalScaleX.getValue(localScaleValX);

	MPlug pluglocalScaleY(thisNode, shapeSizeY);
	MDistance localScaleValY;
	pluglocalScaleY.getValue(localScaleValY);

	MPlug pluglocalScaleZ(thisNode, shapeSizeZ);
	MDistance localScaleValZ;
	pluglocalScaleZ.getValue(localScaleValZ);
	MVector localScale
	(
		localScaleValX.asCentimeters() * mirrorSign,
		localScaleValY.asCentimeters() * mirrorSign,
		localScaleValZ.asCentimeters() * mirrorSign
	);


	// Adjust Vertex Values
	//
	unsigned int vertCount = sizeof(box_VertexList) / sizeof(box_VertexList[0]);
	double AdjustedVerts[512][3];
	unsigned int i;
	for (i = 0; i < vertCount; ++i)
	{
		AdjustedVerts[i][0] = (box_VertexList[i][0] * (localScale.x)) + (localPosition.x );
		AdjustedVerts[i][1] = (box_VertexList[i][1] * (localScale.y)) + (localPosition.y );
		AdjustedVerts[i][2] = (box_VertexList[i][2] * (localScale.z)) + (localPosition.z );
	}


	// Start Legacy Drawing
	//
	view.beginGL();


	if ((style == M3dView::kFlatShaded) ||
		(style == M3dView::kGouraudShaded))
	{
		// Push the color settings
		//
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT |
			GL_PIXEL_MODE_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.5f, 0.5f, 0.0f, 0.2f);

		if (status == M3dView::kActive) {
			view.setDrawColor(2, M3dView::kActiveColors);
		}
		else {
			view.setDrawColor(2, M3dView::kDormantColors);
		}

		glPopAttrib();
	}

	// Draw the outline
	//
	unsigned int edgeCount = sizeof(box_EdgeList) / sizeof(box_EdgeList[0]);
	float miniOffset(1.001f);
	for (i = 0; i < edgeCount; i++)
	{
		glBegin(GL_LINES);
		glVertex3d(
			AdjustedVerts[box_EdgeList[i][0]][0] * miniOffset,
			AdjustedVerts[box_EdgeList[i][0]][1] * miniOffset,
			AdjustedVerts[box_EdgeList[i][0]][2] * miniOffset);
		glVertex3d(
			AdjustedVerts[box_EdgeList[i][1]][0] * miniOffset,
			AdjustedVerts[box_EdgeList[i][1]][1] * miniOffset,
			AdjustedVerts[box_EdgeList[i][1]][2] * miniOffset);
		glEnd();
	}

	view.endGL();

	// Draw the name of the baconShapeBone
	//view.setDrawColor( MColor( 0.1f, 0.8f, 0.8f, 0.2f ) );
	//view.drawText( MString("draw name"), MPoint( 0.0, 0.0, 0.0 ), M3dView::kCenter );
}

bool baconShapeBone::isTransparent() const
{
	MObject thisNode = thisMObject();
	MPlug plug(thisNode, aEnableTransparencySort);
	bool value;
	plug.getValue(value);
	return value;
}

bool baconShapeBone::drawLast() const
{
	MObject thisNode = thisMObject();
	MPlug plug(thisNode, aEnableDrawLast);
	bool value;
	plug.getValue(value);
	return value;
}

bool baconShapeBone::isBounded() const
{
	return true;
}

MBoundingBox baconShapeBone::boundingBox() const
{
	// Get the size
	//
	MObject thisNode = thisMObject();

	// Get the local scale
	//
	MPlug plugLocalMirror(thisNode, mirror);
	bool LocalMirrorVal;
	float mirrorSign;
	plugLocalMirror.getValue(LocalMirrorVal);
	if (LocalMirrorVal == true)
	{
		mirrorSign = -1.0;
	}
	else
	{
		mirrorSign = 1.0;
	}

	// Get the local scale
	//
	MPlug pluglocalScaleX(thisNode, shapeSizeX);
	MDistance localScaleValX;
	pluglocalScaleX.getValue(localScaleValX);

	MPlug pluglocalScaleY(thisNode, shapeSizeY);
	MDistance localScaleValY;
	pluglocalScaleY.getValue(localScaleValY);

	MPlug pluglocalScaleZ(thisNode, shapeSizeZ);
	MDistance localScaleValZ;
	pluglocalScaleZ.getValue(localScaleValZ);
	MVector localScale
	(
		localScaleValX.asCentimeters() * mirrorSign,
		localScaleValY.asCentimeters() * mirrorSign,
		localScaleValZ.asCentimeters() * mirrorSign
	);


	// Get the local Position
	//
	MPlug pluglocalPositionX(thisNode, shapeOffsetX);
	MDistance localPositionValX;
	pluglocalPositionX.getValue(localPositionValX);

	MPlug pluglocalPositionY(thisNode, shapeOffsetY);
	MDistance localPositionValY;
	pluglocalPositionY.getValue(localPositionValY);

	MPlug pluglocalPositionZ(thisNode, shapeOffsetZ);
	MDistance localPositionValZ;
	pluglocalPositionZ.getValue(localPositionValZ);
	MVector localPosition
	(
		localPositionValX.asCentimeters() * mirrorSign,
		localPositionValY.asCentimeters() * mirrorSign,
		localPositionValZ.asCentimeters() * mirrorSign
	);

	// Get the local Rotation
	//
	MPlug pluglocalRotationX(thisNode, shapeOrientationX);
	double localRotationValX;
	pluglocalRotationX.getValue(localRotationValX);

	MPlug pluglocalRotationY(thisNode, shapeOrientationY);
	double localRotationValY;
	pluglocalRotationY.getValue(localRotationValY);

	MPlug pluglocalRotationZ(thisNode, shapeOrientationZ);
	double localRotationValZ;
	pluglocalRotationZ.getValue(localRotationValZ);

	MEulerRotation localRotation(localRotationValX, localRotationValY, localRotationValZ);
	MMatrix rotMatrix(localRotation.asMatrix());

	MPoint corner1(localScale.x / -1.0f, localScale.y / -1.0f, localScale.z / -1.0f);
	MPoint corner2(localScale.x / 1.0f, localScale.y / 1.0f, localScale.z / 1.0f);

	if (localRotationValX != 0.0 || localRotationValY != 0.0 || localRotationValZ != 0.0)
	{
		corner1 = corner1 * rotMatrix;
		corner2 = corner2 * rotMatrix;
	}
	return MBoundingBox(corner1 + localPosition, corner2 + localPosition);
}

// Is this evaluator capable of evaluating clusters in parallel?
MPxNode::SchedulingType baconShapeBone::schedulingType(const MPxNode* update)
{
	return update->schedulingType();
}


void* baconShapeBone::creator()
{
	return new baconShapeBone();
}

*/
double baconShapeBone::doExpensiveCalculation(int c, double t)
{
	// Depending on your machine speed, you may have to tweak the
	// calculation or add a sleep to get Maya to slow down
	//
	unsigned int end = c * c * c * c * c;
	double result = 0;
	for (unsigned int i = 0; i < end; i++)
		result = result + i * c * t;
	result = fmod(result, 7.0) + 1.0;
	return result;
}



MStatus baconShapeBone::postEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType)
{
	if (!context.isNormal())
		return MStatus::kFailure;
	MStatus status;
	if (evalType == kLeaveDirty)
	{
		scaleUpToDate = false;
	}
	else if ((evaluationNode.dirtyPlugExists(aCopies, &status) && status) ||
		(evaluationNode.dirtyPlugExists(aTimeInput, &status) && status))
	{
		MDataBlock block = forceCache();
		MDataHandle inputTimeData = block.inputValue(aTimeInput, &status);
		if (status)
		{
			MDataHandle copiesData = block.inputValue(aCopies, &status);
			if (status)
			{
				// A made up calculation to slow down processing
				//
				MTime time = inputTimeData.asTime();
				int copies = copiesData.asInt();
				double t = time.value();
				if (!scaleUpToDate)
				{
					scaleXBy = doExpensiveCalculation(copies, t);
					// Mark the scale as up to date so that draw does not
					// have to recompute it
					scaleUpToDate = true;
				}
			}
		}
	}
	return MStatus::kSuccess;
}





//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class baconShapeBoneData : public MUserData
{
public:
	baconShapeBoneData() : MUserData(false) {} // don't delete after draw
	virtual ~baconShapeBoneData() {}

	MColor fColor;
	float fWireVisibility;
	float fWireThickness;
	float fSolidVisibility;
	bool fXRay;
	bool fmirror;
	bool fpresto;
	int fShapeType;
	int fMarionetteType;
	MPointArray fShapeLineList[516];
	MPointArray fShapeTriangleList;
};

class baconShapeBoneDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	static MHWRender::MPxDrawOverride* Creator(const MObject& obj)
	{
		return new baconShapeBoneDrawOverride(obj);
	}

	virtual ~baconShapeBoneDrawOverride();

	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool isBounded(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual MBoundingBox boundingBox(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const;

	virtual MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MHWRender::MFrameContext& frameContext,
		MUserData* oldData);

	virtual bool hasUIDrawables() const { return true; }

	virtual void addUIDrawables(
		const MDagPath& objPath,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext,
		const MUserData* data);

	virtual bool traceCallSequence() const
	{
		// Return true if internal tracing is desired.
		return false;
	}
	virtual void handleTraceMessage(const MString &message) const
	{
		MGlobal::displayInfo("baconShapeBoneDrawOverride: " + message);

		// Some simple custom message formatting.
		fprintf(stderr, "baconShapeBoneDrawOverride: ");
		fprintf(stderr, message.asChar());
		fprintf(stderr, "\n");
	}


private:
	baconShapeBoneDrawOverride(const MObject& obj);
	float getMultiplier(const MDagPath& objPath) const;
	float getWireVisibility(const MDagPath& objPath) const;
	float getWireThickness(const MDagPath& objPath) const;
	float getSolidVisibility(const MDagPath& objPath) const;
	bool  getXRay(const MDagPath& objPath) const;
	bool  getMirror(const MDagPath& objPath) const;
	bool  getPresto(const MDagPath& objPath) const;
	int	  getShapeType(const MDagPath& objPath) const;
	int	  getMarionetteType(const MDagPath& objPath) const;
	MVector getLocalPosition(const MDagPath& objPath) const;
	MVector getLocalScale(const MDagPath& objPath) const;
	MEulerRotation getLocalRotation(const MDagPath& objPath) const;

	static void OnModelEditorChanged(void *clientData);

	baconShapeBone*  fbaconShapeBone;
	MCallbackId fModelEditorChangedCbId;
};

// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
// draw override will be updated (via prepareForDraw()) only when the node
// is marked dirty via DG evaluation or dirty propagation. Additional
// callback is also added to explicitly mark the node as being dirty (via
// MRenderer::setGeometryDrawDirty()) for certain circumstances. Note that
// the draw callback in MPxDrawOverride constructor is set to NULL in order
// to achieve better performance.
baconShapeBoneDrawOverride::baconShapeBoneDrawOverride(const MObject& obj)
	: MHWRender::MPxDrawOverride(obj, NULL, false)
{
	fModelEditorChangedCbId = MEventMessage::addEventCallback(
		"modelEditorChanged", OnModelEditorChanged, this);

	MStatus status;
	MFnDependencyNode node(obj, &status);
	fbaconShapeBone = status ? dynamic_cast<baconShapeBone*>(node.userNode()) : NULL;
}

baconShapeBoneDrawOverride::~baconShapeBoneDrawOverride()
{
	fbaconShapeBone = NULL;

	if (fModelEditorChangedCbId != 0)
	{
		MMessage::removeCallback(fModelEditorChangedCbId);
		fModelEditorChangedCbId = 0;
	}
}

void baconShapeBoneDrawOverride::OnModelEditorChanged(void *clientData)
{
	// Mark the node as being dirty so that it can update on display appearance
	// switch among wireframe and shaded.
	baconShapeBoneDrawOverride *ovr = static_cast<baconShapeBoneDrawOverride*>(clientData);
	if (ovr && ovr->fbaconShapeBone)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(ovr->fbaconShapeBone->thisMObject());
	}
}

MHWRender::DrawAPI baconShapeBoneDrawOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

MVector baconShapeBoneDrawOverride::getLocalPosition(const MDagPath& objPath) const
{
	// Retrieve value of the local position attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	MVector returnValue(0.0, 0.0, 0.0);
	float mirrorValue(1.0);
	if (status)
	{
		MPlug plugMirror(baconShapeBoneNode, baconShapeBone::mirror);
		if (plugMirror.asBool())
		{
			mirrorValue = -1.0;
		}

		MPlug plugX(baconShapeBoneNode, baconShapeBone::shapeOffsetX);
		if (!plugX.isNull())
		{
			MDistance localPositionXVal;
			if (plugX.getValue(localPositionXVal))
			{
				returnValue.x = localPositionXVal.asCentimeters() * mirrorValue;
			}
		}
		MPlug plugY(baconShapeBoneNode, baconShapeBone::shapeOffsetY);
		if (!plugY.isNull())
		{
			MDistance localPositionYVal;
			if (plugY.getValue(localPositionYVal))
			{
				returnValue.y = localPositionYVal.asCentimeters() * mirrorValue;
			}
		}
		MPlug plugZ(baconShapeBoneNode, baconShapeBone::shapeOffsetZ);
		if (!plugZ.isNull())
		{
			MDistance localPositionZVal;
			if (plugZ.getValue(localPositionZVal))
			{
				returnValue.z = localPositionZVal.asCentimeters() * mirrorValue;
			}
		}
	}

	return returnValue;
}

MVector baconShapeBoneDrawOverride::getLocalScale(const MDagPath& objPath) const
{
	// Retrieve value of the local position attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	MVector returnValue(0.0, 0.0, 0.0);
	float mirrorValue(1.0);
	if (status)
	{
		MPlug plugMirror(baconShapeBoneNode, baconShapeBone::mirror);
		if (plugMirror.asBool())
		{
			mirrorValue = -1.0;
		}

		MPlug plugX(baconShapeBoneNode, baconShapeBone::shapeSizeX);
		if (!plugX.isNull())
		{
			MDistance localScaleXVal;
			if (plugX.getValue(localScaleXVal))
			{
				returnValue.x = localScaleXVal.asCentimeters() * mirrorValue;
			}
		}
		MPlug plugY(baconShapeBoneNode, baconShapeBone::shapeSizeY);
		if (!plugY.isNull())
		{
			MDistance localScaleYVal;
			if (plugY.getValue(localScaleYVal))
			{
				returnValue.y = localScaleYVal.asCentimeters() * mirrorValue;
			}
		}
		MPlug plugZ(baconShapeBoneNode, baconShapeBone::shapeSizeZ);
		if (!plugZ.isNull())
		{
			MDistance localScaleZVal;
			if (plugZ.getValue(localScaleZVal))
			{
				returnValue.z = localScaleZVal.asCentimeters() * mirrorValue;
			}
		}
	}

	return returnValue;
}

MEulerRotation baconShapeBoneDrawOverride::getLocalRotation(const MDagPath& objPath) const
{
	// Retrieve value of the local position attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	MEulerRotation returnValue(0, 0, 0);
	if (status)
	{
		MPlug plugX(baconShapeBoneNode, baconShapeBone::shapeOrientationX);
		if (!plugX.isNull())
		{
			double localRotationXVal;
			if (plugX.getValue(localRotationXVal))
			{
				returnValue.x = localRotationXVal;
			}
		}
		MPlug plugY(baconShapeBoneNode, baconShapeBone::shapeOrientationY);
		if (!plugY.isNull())
		{
			double localRotationYVal;
			if (plugY.getValue(localRotationYVal))
			{
				returnValue.y = localRotationYVal;
			}
		}
		MPlug plugZ(baconShapeBoneNode, baconShapeBone::shapeOrientationZ);
		if (!plugZ.isNull())
		{
			double localRotationZVal;
			if (plugZ.getValue(localRotationZVal))
			{
				returnValue.z = localRotationZVal;
			}
		}
	}
	return returnValue;
}

float baconShapeBoneDrawOverride::getMultiplier(const MDagPath& objPath) const
{
	// Retrieve value of the size attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::size);
		if (!plug.isNull())
		{
			MDistance sizeVal;
			if (plug.getValue(sizeVal))
			{
				return (float)sizeVal.asCentimeters();
			}
		}
	}

	return 1.0f;
}

float baconShapeBoneDrawOverride::getWireVisibility(const MDagPath& objPath) const
{
	// Retrieve value of the WireColor attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::wireVisibility);
		if (!plug.isNull())
		{
			MDistance wireVisibilityVal;
			if (plug.getValue(wireVisibilityVal))
			{
				return (float)wireVisibilityVal.asCentimeters();
			}
		}
	}
	return 1.0f;
}

float baconShapeBoneDrawOverride::getWireThickness(const MDagPath& objPath) const
{
	// Retrieve value of the WireColor attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::wireThickness);
		if (!plug.isNull())
		{
			MDistance wireThicknessVal;
			if (plug.getValue(wireThicknessVal))
			{
				return (float)wireThicknessVal.asCentimeters();
			}
		}
	}
	return 1.0f;
}

float baconShapeBoneDrawOverride::getSolidVisibility(const MDagPath& objPath) const
{
	// Retrieve value of the WireColor attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::solidVisibility);
		if (!plug.isNull())
		{
			MDistance solidVisibilityVal;
			if (plug.getValue(solidVisibilityVal))
			{
				return (float)solidVisibilityVal.asCentimeters();
			}
		}
	}
	return 1.0f;
}

bool baconShapeBoneDrawOverride::getXRay(const MDagPath& objPath) const
{
	// Retrieve value of the xRay attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::xRay);
		if (!plug.isNull())
		{
			bool xrayVal;
			if (plug.getValue(xrayVal))
			{
				return plug.asBool();
			}
		}
	}
	return false;
}

bool baconShapeBoneDrawOverride::getMirror(const MDagPath& objPath) const
{
	// Retrieve value of the mirror attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::mirror);
		if (!plug.isNull())
		{
			bool mirrorVal;
			if (plug.getValue(mirrorVal))
			{
				return plug.asBool();
			}
		}
	}
	return false;
}

bool baconShapeBoneDrawOverride::getPresto(const MDagPath& objPath) const
{
	// Retrieve value of the Presto Selection attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::prestoMode);
		if (!plug.isNull())
		{
			bool prestoVal;
			if (plug.getValue(prestoVal))
			{
				return plug.asBool();
			}
		}
	}
	return false;
}

int baconShapeBoneDrawOverride::getShapeType(const MDagPath& objPath) const
{
	// Retrieve value of the shapeType attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::shapeType);
		if (!plug.isNull())
		{
			return plug.asInt();
		}
	}
	return 0;
}

int baconShapeBoneDrawOverride::getMarionetteType(const MDagPath& objPath) const
{
	// Retrieve value of the marionetteType attribute from the node
	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	if (status)
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::marionetteType);
		if (!plug.isNull())
		{
			return plug.asInt();
		}
	}
	return 0;
}


bool baconShapeBoneDrawOverride::isBounded(const MDagPath& /*objPath*/,
	const MDagPath& /*cameraPath*/) const
{
	return true;
}

MBoundingBox baconShapeBoneDrawOverride::boundingBox(
	const MDagPath& objPath,
	const MDagPath& cameraPath) const
{
	MVector scaleAdjust = getLocalScale(objPath);
	MVector offsetAdjust = getLocalPosition(objPath);
	MEulerRotation rotAdjust = getLocalRotation(objPath);
	//int ShapeID = getShapeType(objPath);
	//MBoundingBox returnbox;

	MPoint corner1(scaleAdjust.x / -1.0f, scaleAdjust.y / -1.0f, scaleAdjust.z / -1.0f);
	MPoint corner2(scaleAdjust.x / 1.0f, scaleAdjust.y / 1.0f, scaleAdjust.z / 1.0f);

	if (rotAdjust.x != 0.0 || rotAdjust.y != 0.0 || rotAdjust.z != 0.0)
	{
		MMatrix rotMatrix(rotAdjust.asMatrix());
		corner1 = corner1 * rotMatrix;
		corner2 = corner2 * rotMatrix;
	}

	//MPoint corner1(0.0f, scaleAdjust.y / -2.0f, scaleAdjust.z / -2.0f);
	//MPoint corner2(scaleAdjust.x, scaleAdjust.y / 2.0f, scaleAdjust.z / 2.0f);
	MBoundingBox returnbox(corner1 + offsetAdjust, corner2 + offsetAdjust);

	//float multiplier = getMultiplier(objPath);


	return returnbox;
}


// Called by Maya each time the object needs to be drawn.
MUserData* baconShapeBoneDrawOverride::prepareForDraw(
	const MDagPath& objPath,
	const MDagPath& cameraPath,
	const MHWRender::MFrameContext& frameContext,
	MUserData* oldData)
{
	// Any data needed from the Maya dependency graph must be retrieved and cached in this stage.
	// There is one cache data for each drawable instance, if it is not desirable to allow Maya to handle data
	// caching, simply return null in this method and ignore user data parameter in draw callback method.
	// e.g. in this sample, we compute and cache the data for usage later when we create the 
	// MUIDrawManager to draw baconShapeBone in method addUIDrawables().
	baconShapeBoneData* data = dynamic_cast<baconShapeBoneData*>(oldData);
	if (!data)
	{
		data = new baconShapeBoneData();
	}

	MStatus status;
	MObject baconShapeBoneNode = objPath.node(&status);
	float fMultiplier = getMultiplier(objPath);

	// retrieve xRay flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::xRay);
		data->fXRay = plug.asBool();
	}

	// retrieve mirror flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::mirror);
		data->fmirror = plug.asBool();
	}

	// retrieve Presto flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::prestoMode);
		data->fpresto = plug.asBool();
	}

	// retrieve shapeType flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::shapeType);
		data->fShapeType = plug.asInt();
	}

	// retrieve marionetteType flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::marionetteType);
		data->fMarionetteType = plug.asInt();
	}

	// retrieve wireVisibility flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::wireVisibility);
		data->fWireVisibility = plug.asFloat();
	}

	// retrieve wireThickness flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::wireThickness);
		data->fWireThickness = plug.asFloat();
	}

	// retrieve solidVisibility flag
	{
		MPlug plug(baconShapeBoneNode, baconShapeBone::solidVisibility);
		data->fSolidVisibility = plug.asFloat();
	}


	MVector positionAdjust = getLocalPosition(objPath);
	MVector scaleAdjust = getLocalScale(objPath);
	MEulerRotation rotAdjust = getLocalRotation(objPath);
	bool isRotated = (rotAdjust.x != 0 || rotAdjust.y != 0 || rotAdjust.z != 0);
	MMatrix rotMatrix = rotAdjust.asMatrix();


	// Set Buffer with Choice of Shape
	//

	MVector	outputVerts[512];
	int3	outputFaces[512];
	int2	outputEdges[512];
	unsigned int vertCount = 0;
	unsigned int faceCount = 0;
	unsigned int edgeCount = 0;
	unsigned int i;

	if (data->fShapeType == 1) //Capsule
	{
		vertCount = sizeof(Capsule_VertexList) / sizeof(Capsule_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Capsule_VertexList[i][0], Capsule_VertexList[i][1], Capsule_VertexList[i][2]);
		}

		faceCount = sizeof(Capsule_FaceList) / sizeof(Capsule_FaceList[0]);
		memcpy(outputFaces, Capsule_FaceList, sizeof(Capsule_FaceList));

		edgeCount = sizeof(Capsule_EdgeList) / sizeof(Capsule_EdgeList[0]);
		memcpy(outputEdges, Capsule_EdgeList, sizeof(Capsule_EdgeList));
	}
	else if (data->fShapeType == 2) //Cylinder
	{
		vertCount = sizeof(Cylinder_VertexList) / sizeof(Cylinder_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Cylinder_VertexList[i][0], Cylinder_VertexList[i][1], Cylinder_VertexList[i][2]);
		}

		faceCount = sizeof(Cylinder_FaceList) / sizeof(Cylinder_FaceList[0]);
		memcpy(outputFaces, Cylinder_FaceList, sizeof(Cylinder_FaceList));

		edgeCount = sizeof(Cylinder_EdgeList) / sizeof(Cylinder_EdgeList[0]);
		memcpy(outputEdges, Cylinder_EdgeList, sizeof(Cylinder_EdgeList));
	}
	else if (data->fShapeType == 3) //Peg
	{
		vertCount = sizeof(Peg_VertexList) / sizeof(Peg_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Peg_VertexList[i][0], Peg_VertexList[i][1], Peg_VertexList[i][2]);
		}

		faceCount = sizeof(Peg_FaceList) / sizeof(Peg_FaceList[0]);
		memcpy(outputFaces, Peg_FaceList, sizeof(Peg_FaceList));

		edgeCount = sizeof(Peg_EdgeList) / sizeof(Peg_EdgeList[0]);
		memcpy(outputEdges, Peg_EdgeList, sizeof(Peg_EdgeList));
	}
	else if (data->fShapeType == 4) //ball
	{
		vertCount = sizeof(Sphere_VertexList) / sizeof(Sphere_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Sphere_VertexList[i][0], Sphere_VertexList[i][1], Sphere_VertexList[i][2]);
		}

		faceCount = sizeof(Sphere_FaceList) / sizeof(Sphere_FaceList[0]);
		memcpy(outputFaces, Sphere_FaceList, sizeof(Sphere_FaceList));

		edgeCount = sizeof(Sphere_EdgeList) / sizeof(Sphere_EdgeList[0]);
		memcpy(outputEdges, Sphere_EdgeList, sizeof(Sphere_EdgeList));
	}
	else if (data->fShapeType == 5) //Circle
	{
		vertCount = sizeof(Circle_VertexList) / sizeof(Circle_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Circle_VertexList[i][0], Circle_VertexList[i][1], Circle_VertexList[i][2]);
		}

		faceCount = sizeof(Circle_FaceList) / sizeof(Circle_FaceList[0]);
		memcpy(outputFaces, Circle_FaceList, sizeof(Circle_FaceList));

		edgeCount = sizeof(Circle_EdgeList) / sizeof(Circle_EdgeList[0]);
		memcpy(outputEdges, Circle_EdgeList, sizeof(Circle_EdgeList));
	}
	else if (data->fShapeType == 6) //Square
	{
		vertCount = sizeof(Square_VertexList) / sizeof(Square_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Square_VertexList[i][0], Square_VertexList[i][1], Square_VertexList[i][2]);
		}

		faceCount = sizeof(Square_FaceList) / sizeof(Square_FaceList[0]);
		memcpy(outputFaces, Square_FaceList, sizeof(Square_FaceList));

		edgeCount = sizeof(Square_EdgeList) / sizeof(Square_EdgeList[0]);
		memcpy(outputEdges, Square_EdgeList, sizeof(Square_EdgeList));
	}
	else if (data->fShapeType == 7) //Locator
	{
		vertCount = sizeof(Locator_VertexList) / sizeof(Locator_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Locator_VertexList[i][0], Locator_VertexList[i][1], Locator_VertexList[i][2]);
		}
		edgeCount = sizeof(Locator_EdgeList) / sizeof(Locator_EdgeList[0]);
		memcpy(outputEdges, Locator_EdgeList, sizeof(Locator_EdgeList));
	}
	else if (data->fShapeType == 8) //Loop
	{
		vertCount = sizeof(Loop_VertexList) / sizeof(Loop_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Loop_VertexList[i][0], Loop_VertexList[i][1], Loop_VertexList[i][2]);
		}

		faceCount = sizeof(Loop_FaceList) / sizeof(Loop_FaceList[0]);
		memcpy(outputFaces, Loop_FaceList, sizeof(Loop_FaceList));

		edgeCount = sizeof(Loop_EdgeList) / sizeof(Loop_EdgeList[0]);
		memcpy(outputEdges, Loop_EdgeList, sizeof(Loop_EdgeList));
	}
	else if (data->fShapeType == 9) //Disc
	{
		vertCount = sizeof(disc_VertexList) / sizeof(disc_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(disc_VertexList[i][0], disc_VertexList[i][1], disc_VertexList[i][2]);
		}

		faceCount = sizeof(disc_FaceList) / sizeof(disc_FaceList[0]);
		memcpy(outputFaces, disc_FaceList, sizeof(disc_FaceList));

		edgeCount = sizeof(disc_EdgeList) / sizeof(disc_EdgeList[0]);
		memcpy(outputEdges, disc_EdgeList, sizeof(disc_EdgeList));
	}
	else if (data->fShapeType == 10) //Ring
	{
		vertCount = sizeof(Ring_VertexList) / sizeof(Ring_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Ring_VertexList[i][0], Ring_VertexList[i][1], Ring_VertexList[i][2]);
		}

		faceCount = sizeof(Ring_FaceList) / sizeof(Ring_FaceList[0]);
		memcpy(outputFaces, Ring_FaceList, sizeof(Ring_FaceList));

		edgeCount = sizeof(Ring_EdgeList) / sizeof(Ring_EdgeList[0]);
		memcpy(outputEdges, Ring_EdgeList, sizeof(Ring_EdgeList));
	}
	else if (data->fShapeType == 11) //Plus
	{
		vertCount = sizeof(Plus_VertexList) / sizeof(Plus_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Plus_VertexList[i][0], Plus_VertexList[i][1], Plus_VertexList[i][2]);
		}

		faceCount = sizeof(Plus_FaceList) / sizeof(Plus_FaceList[0]);
		memcpy(outputFaces, Plus_FaceList, sizeof(Plus_FaceList));

		edgeCount = sizeof(Plus_EdgeList) / sizeof(Plus_EdgeList[0]);
		memcpy(outputEdges, Plus_EdgeList, sizeof(Plus_EdgeList));
	}
	else if (data->fShapeType == 12) //Chevron
	{
		vertCount = sizeof(Chevron_VertexList) / sizeof(Chevron_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Chevron_VertexList[i][0], Chevron_VertexList[i][1], Chevron_VertexList[i][2]);
		}

		faceCount = sizeof(Chevron_FaceList) / sizeof(Chevron_FaceList[0]);
		memcpy(outputFaces, Chevron_FaceList, sizeof(Chevron_FaceList));

		edgeCount = sizeof(Chevron_EdgeList) / sizeof(Chevron_EdgeList[0]);
		memcpy(outputEdges, Chevron_EdgeList, sizeof(Chevron_EdgeList));
	}
	else if (data->fShapeType == 13) //FourWay
	{
		vertCount = sizeof(FourWay_VertexList) / sizeof(FourWay_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(FourWay_VertexList[i][0], FourWay_VertexList[i][1], FourWay_VertexList[i][2]);
		}

		faceCount = sizeof(FourWay_FaceList) / sizeof(FourWay_FaceList[0]);
		memcpy(outputFaces, FourWay_FaceList, sizeof(FourWay_FaceList));

		edgeCount = sizeof(FourWay_EdgeList) / sizeof(FourWay_EdgeList[0]);
		memcpy(outputEdges, FourWay_EdgeList, sizeof(FourWay_EdgeList));
	}
	else if (data->fShapeType == 14) //Arrowhead
	{
		vertCount = sizeof(Pyramid_VertexList) / sizeof(Pyramid_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Pyramid_VertexList[i][0], Pyramid_VertexList[i][1], Pyramid_VertexList[i][2]);
		}

		faceCount = sizeof(Pyramid_FaceList) / sizeof(Pyramid_FaceList[0]);
		memcpy(outputFaces, Pyramid_FaceList, sizeof(Pyramid_FaceList));

		edgeCount = sizeof(Pyramid_EdgeList) / sizeof(Pyramid_EdgeList[0]);
		memcpy(outputEdges, Pyramid_EdgeList, sizeof(Pyramid_EdgeList));
	}
	else if (data->fShapeType == 15) //Diamond
	{
		vertCount = sizeof(Diamond_VertexList) / sizeof(Diamond_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Diamond_VertexList[i][0], Diamond_VertexList[i][1], Diamond_VertexList[i][2]);
		}

		faceCount = sizeof(Diamond_FaceList) / sizeof(Diamond_FaceList[0]);
		memcpy(outputFaces, Diamond_FaceList, sizeof(Diamond_FaceList));

		edgeCount = sizeof(Diamond_EdgeList) / sizeof(Diamond_EdgeList[0]);
		memcpy(outputEdges, Diamond_EdgeList, sizeof(Diamond_EdgeList));
	}
	else if (data->fShapeType == 16) //Heart
	{
		vertCount = sizeof(heart_VertexList) / sizeof(heart_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(heart_VertexList[i][0], heart_VertexList[i][1], heart_VertexList[i][2]);
		}

		faceCount = sizeof(heart_FaceList) / sizeof(heart_FaceList[0]);
		memcpy(outputFaces, heart_FaceList, sizeof(heart_FaceList));

		edgeCount = sizeof(heart_EdgeList) / sizeof(heart_EdgeList[0]);
		memcpy(outputEdges, heart_EdgeList, sizeof(heart_EdgeList));
	}
	else if (data->fShapeType == 17) //pin
	{
		vertCount = sizeof(Sphere_VertexList) / sizeof(Sphere_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Sphere_VertexList[i][0], Sphere_VertexList[i][1], Sphere_VertexList[i][2]);
		}

		faceCount = sizeof(Sphere_FaceList) / sizeof(Sphere_FaceList[0]);
		memcpy(outputFaces, Sphere_FaceList, sizeof(Sphere_FaceList));

		edgeCount = sizeof(Sphere_EdgeList) / sizeof(Sphere_EdgeList[0]);
		memcpy(outputEdges, Sphere_EdgeList, sizeof(Sphere_EdgeList));
	}
	else if (data->fShapeType == 18) //Marionette Part
	{
		if (data->fMarionetteType == 1) //CSPelvis
		{
			vertCount = sizeof(CSPelvis_VertexList) / sizeof(CSPelvis_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSPelvis_VertexList[i][0], CSPelvis_VertexList[i][1], CSPelvis_VertexList[i][2]);
			}

			faceCount = sizeof(CSPelvis_FaceList) / sizeof(CSPelvis_FaceList[0]);
			memcpy(outputFaces, CSPelvis_FaceList, sizeof(CSPelvis_FaceList));

			edgeCount = sizeof(CSPelvis_EdgeList) / sizeof(CSPelvis_EdgeList[0]);
			memcpy(outputEdges, CSPelvis_EdgeList, sizeof(CSPelvis_EdgeList));
		}
		else if (data->fMarionetteType == 2) //CSSpine
		{
			vertCount = sizeof(CSSpine_VertexList) / sizeof(CSSpine_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSSpine_VertexList[i][0], CSSpine_VertexList[i][1], CSSpine_VertexList[i][2]);
			}

			faceCount = sizeof(CSSpine_FaceList) / sizeof(CSSpine_FaceList[0]);
			memcpy(outputFaces, CSSpine_FaceList, sizeof(CSSpine_FaceList));

			edgeCount = sizeof(CSSpine_EdgeList) / sizeof(CSSpine_EdgeList[0]);
			memcpy(outputEdges, CSSpine_EdgeList, sizeof(CSSpine_EdgeList));
		}
		else if (data->fMarionetteType == 3) //CSClavicle
		{
			vertCount = sizeof(CSClavicle_VertexList) / sizeof(CSClavicle_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSClavicle_VertexList[i][0], CSClavicle_VertexList[i][1], CSClavicle_VertexList[i][2]);
			}

			faceCount = sizeof(CSClavicle_FaceList) / sizeof(CSClavicle_FaceList[0]);
			memcpy(outputFaces, CSClavicle_FaceList, sizeof(CSClavicle_FaceList));

			edgeCount = sizeof(CSClavicle_EdgeList) / sizeof(CSClavicle_EdgeList[0]);
			memcpy(outputEdges, CSClavicle_EdgeList, sizeof(CSClavicle_EdgeList));
		}
		else if (data->fMarionetteType == 4) //CSUpperArm
		{
			vertCount = sizeof(CSUpperArm_VertexList) / sizeof(CSUpperArm_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSUpperArm_VertexList[i][0], CSUpperArm_VertexList[i][1], CSUpperArm_VertexList[i][2]);
			}

			faceCount = sizeof(CSUpperArm_FaceList) / sizeof(CSUpperArm_FaceList[0]);
			memcpy(outputFaces, CSUpperArm_FaceList, sizeof(CSUpperArm_FaceList));

			edgeCount = sizeof(CSUpperArm_EdgeList) / sizeof(CSUpperArm_EdgeList[0]);
			memcpy(outputEdges, CSUpperArm_EdgeList, sizeof(CSUpperArm_EdgeList));
		}
		else if (data->fMarionetteType == 5) //CSForearm
		{
			vertCount = sizeof(CSForearm_VertexList) / sizeof(CSForearm_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSForearm_VertexList[i][0], CSForearm_VertexList[i][1], CSForearm_VertexList[i][2]);
			}

			faceCount = sizeof(CSForearm_FaceList) / sizeof(CSForearm_FaceList[0]);
			memcpy(outputFaces, CSForearm_FaceList, sizeof(CSForearm_FaceList));

			edgeCount = sizeof(CSForearm_EdgeList) / sizeof(CSForearm_EdgeList[0]);
			memcpy(outputEdges, CSForearm_EdgeList, sizeof(CSForearm_EdgeList));
		}
		else if (data->fMarionetteType == 6) //CSHand
		{
			vertCount = sizeof(CSHand_VertexList) / sizeof(CSHand_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSHand_VertexList[i][0], CSHand_VertexList[i][1], CSHand_VertexList[i][2]);
			}

			faceCount = sizeof(CSHand_FaceList) / sizeof(CSHand_FaceList[0]);
			memcpy(outputFaces, CSHand_FaceList, sizeof(CSHand_FaceList));

			edgeCount = sizeof(CSHand_EdgeList) / sizeof(CSHand_EdgeList[0]);
			memcpy(outputEdges, CSHand_EdgeList, sizeof(CSHand_EdgeList));
		}
		else if (data->fMarionetteType == 7) //CSThigh
		{
			vertCount = sizeof(CSThigh_VertexList) / sizeof(CSThigh_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSThigh_VertexList[i][0], CSThigh_VertexList[i][1], CSThigh_VertexList[i][2]);
			}

			faceCount = sizeof(CSThigh_FaceList) / sizeof(CSThigh_FaceList[0]);
			memcpy(outputFaces, CSThigh_FaceList, sizeof(CSThigh_FaceList));

			edgeCount = sizeof(CSThigh_EdgeList) / sizeof(CSThigh_EdgeList[0]);
			memcpy(outputEdges, CSThigh_EdgeList, sizeof(CSThigh_EdgeList));
		}
		else if (data->fMarionetteType == 8) //CSCalf
		{
			vertCount = sizeof(CSCalf_VertexList) / sizeof(CSCalf_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSCalf_VertexList[i][0], CSCalf_VertexList[i][1], CSCalf_VertexList[i][2]);
			}

			faceCount = sizeof(CSCalf_FaceList) / sizeof(CSCalf_FaceList[0]);
			memcpy(outputFaces, CSCalf_FaceList, sizeof(CSCalf_FaceList));

			edgeCount = sizeof(CSCalf_EdgeList) / sizeof(CSCalf_EdgeList[0]);
			memcpy(outputEdges, CSCalf_EdgeList, sizeof(CSCalf_EdgeList));
		}
		else if (data->fMarionetteType == 9) //CSFoot
		{
			vertCount = sizeof(CSFoot_VertexList) / sizeof(CSFoot_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSFoot_VertexList[i][0], CSFoot_VertexList[i][1], CSFoot_VertexList[i][2]);
			}

			faceCount = sizeof(CSFoot_FaceList) / sizeof(CSFoot_FaceList[0]);
			memcpy(outputFaces, CSFoot_FaceList, sizeof(CSFoot_FaceList));

			edgeCount = sizeof(CSFoot_EdgeList) / sizeof(CSFoot_EdgeList[0]);
			memcpy(outputEdges, CSFoot_EdgeList, sizeof(CSFoot_EdgeList));
		}
		else //CS Head
		{
			vertCount = sizeof(CSHead_VertexList) / sizeof(CSHead_VertexList[0]);
			for (i = 0; i < vertCount; ++i)
			{
				outputVerts[i] = MVector(CSHead_VertexList[i][0], CSHead_VertexList[i][1], CSHead_VertexList[i][2]);
			}

			faceCount = sizeof(CSHead_FaceList) / sizeof(CSHead_FaceList[0]);
			memcpy(outputFaces, CSHead_FaceList, sizeof(CSHead_FaceList));

			edgeCount = sizeof(CSHead_EdgeList) / sizeof(CSHead_EdgeList[0]);
			memcpy(outputEdges, CSHead_EdgeList, sizeof(CSHead_EdgeList));
		}
	}
	else if (data->fShapeType == 19) //Goggles
	{
		vertCount = sizeof(Goggles_VertexList) / sizeof(Goggles_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Goggles_VertexList[i][0], Goggles_VertexList[i][1], Goggles_VertexList[i][2]);
		}

		faceCount = sizeof(Goggles_FaceList) / sizeof(Goggles_FaceList[0]);
		memcpy(outputFaces, Goggles_FaceList, sizeof(Goggles_FaceList));

		edgeCount = sizeof(Goggles_EdgeList) / sizeof(Goggles_EdgeList[0]);
		memcpy(outputEdges, Goggles_EdgeList, sizeof(Goggles_EdgeList));
	}
	else if (data->fShapeType == 20) //ShoulderPad
	{
		vertCount = sizeof(ShoulderPad_VertexList) / sizeof(ShoulderPad_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(ShoulderPad_VertexList[i][0], ShoulderPad_VertexList[i][1], ShoulderPad_VertexList[i][2]);
		}

		faceCount = sizeof(ShoulderPad_FaceList) / sizeof(ShoulderPad_FaceList[0]);
		memcpy(outputFaces, ShoulderPad_FaceList, sizeof(ShoulderPad_FaceList));

		edgeCount = sizeof(ShoulderPad_EdgeList) / sizeof(ShoulderPad_EdgeList[0]);
		memcpy(outputEdges, ShoulderPad_EdgeList, sizeof(ShoulderPad_EdgeList));
	}
	else if (data->fShapeType == 21) //Feather
	{
		vertCount = sizeof(Feather_VertexList) / sizeof(Feather_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(Feather_VertexList[i][0], Feather_VertexList[i][1], Feather_VertexList[i][2]);
		}

		faceCount = sizeof(Feather_FaceList) / sizeof(Feather_FaceList[0]);
		memcpy(outputFaces, Feather_FaceList, sizeof(Feather_FaceList));

		edgeCount = sizeof(Feather_EdgeList) / sizeof(Feather_EdgeList[0]);
		memcpy(outputEdges, Feather_EdgeList, sizeof(Feather_EdgeList));
	}
	else
	{
		vertCount = sizeof(box_VertexList) / sizeof(box_VertexList[0]);
		for (i = 0; i < vertCount; ++i)
		{
			outputVerts[i] = MVector(box_VertexList[i][0], box_VertexList[i][1], box_VertexList[i][2]);
		}

		faceCount = sizeof(box_FaceList) / sizeof(box_FaceList[0]);
		memcpy(outputFaces, box_FaceList, sizeof(box_FaceList));

		edgeCount = sizeof(box_EdgeList) / sizeof(box_EdgeList[0]);
		memcpy(outputEdges, box_EdgeList, sizeof(box_EdgeList));
	}

	// Adjust Vertex Values
	//
	if (data->fShapeType == 1) //capsule scaling works differently
	{
		int unsigned halfVerts(vertCount / 2);
		double halfOffset = scaleAdjust[0];
		double offsetSign = 1.;
		if (halfOffset < 0) offsetSign = -1.;


		for (i = 0; i < vertCount; ++i)
		{
			double adjustedX;
			double adjustedY;
			double adjustedZ;

			if (i < halfVerts)
			{
				adjustedX = (outputVerts[i][0] * scaleAdjust[1]);
				adjustedY = (outputVerts[i][1] * scaleAdjust[1]);
				adjustedZ = (outputVerts[i][2] * scaleAdjust[1]);
				if (offsetSign >= 0.0) adjustedX += halfOffset;
			}
			else
			{
				adjustedX = (outputVerts[i][0] * scaleAdjust[2]);
				adjustedY = (outputVerts[i][1] * scaleAdjust[2]);
				adjustedZ = (outputVerts[i][2] * scaleAdjust[2]);
				if (offsetSign < 0.0) adjustedX += halfOffset;
			}


			MVector adjustedVert(adjustedX, adjustedY, adjustedZ);

			if (isRotated)
			{
				adjustedVert = adjustedVert * rotMatrix;
			}
			outputVerts[i][0] = adjustedVert.x + positionAdjust[0];
			outputVerts[i][1] = adjustedVert.y + positionAdjust[1];
			outputVerts[i][2] = adjustedVert.z + positionAdjust[2];
		}
	}
	else if (data->fShapeType == 2) //Cylinder scaling works differently
	{
		int unsigned halfVerts(vertCount / 2);
		//double halfOffset = scaleAdjust[0];
		//double offsetSign = 1.;
		//if (halfOffset < 0) offsetSign = -1.;

		for (i = 0; i < vertCount; ++i)
		{
			double adjustedX;
			double adjustedY;
			double adjustedZ;

			if (i < halfVerts)
			{
				adjustedX = (outputVerts[i][0] * scaleAdjust[1]);
				adjustedY = (outputVerts[i][1] * scaleAdjust[1]);
				adjustedZ = (outputVerts[i][2] * scaleAdjust[1]);
				//if (offsetSign >= 0.0) adjustedX += halfOffset;
			}
			else
			{
				adjustedX = (outputVerts[i][0] * scaleAdjust[2]) + scaleAdjust[0];
				adjustedY = (outputVerts[i][1] * scaleAdjust[2]);
				adjustedZ = (outputVerts[i][2] * scaleAdjust[2]);
				//if (offsetSign < 0.0) adjustedX += halfOffset;
			}


			MVector adjustedVert(adjustedX, adjustedY, adjustedZ);

			if (isRotated)
			{
				adjustedVert = adjustedVert * rotMatrix;
			}
			outputVerts[i][0] = adjustedVert.x + positionAdjust[0];
			outputVerts[i][1] = adjustedVert.y + positionAdjust[1];
			outputVerts[i][2] = adjustedVert.z + positionAdjust[2];
		}
	}
	else if (data->fShapeType == 17) //Pin
	{
		for (i = 0; i < vertCount; ++i)
		{
			MVector adjustedVert(
				outputVerts[i][0] * scaleAdjust[0],
				outputVerts[i][1] * scaleAdjust[1],
				outputVerts[i][2] * scaleAdjust[2]
			);

			if (isRotated)
			{
				adjustedVert = adjustedVert * rotMatrix;
			}

			outputVerts[i][0] = adjustedVert.x + positionAdjust[0];
			outputVerts[i][1] = adjustedVert.y + positionAdjust[1];
			outputVerts[i][2] = adjustedVert.z + positionAdjust[2];
		}

		//Make Pin Line
		vertCount = sizeof(Sphere_VertexList) / sizeof(Sphere_VertexList[0]);
		outputVerts[vertCount] = MVector(0.0, 0.0, 0.0);
		outputVerts[vertCount + 1] = MVector(positionAdjust[0], 0.0, 0.0);
		outputVerts[vertCount + 2] = MVector(positionAdjust[0], positionAdjust[1], 0.0);
		outputVerts[vertCount + 3] = MVector(positionAdjust[0], positionAdjust[1], positionAdjust[2]);


		edgeCount = sizeof(Sphere_EdgeList) / sizeof(Sphere_EdgeList[0]);
		outputEdges[edgeCount][0] = vertCount;
		outputEdges[edgeCount][1] = vertCount + 1;
		outputEdges[edgeCount + 1][0] = vertCount + 1;
		outputEdges[edgeCount + 1][1] = vertCount + 2;
		outputEdges[edgeCount + 2][0] = vertCount + 2;
		outputEdges[edgeCount + 2][1] = vertCount + 3;
		
		vertCount = vertCount + 3;
		edgeCount = edgeCount + 3;

	}
	else //everything else that uses a standard transform.
	{
		for (i = 0; i < vertCount; ++i)
		{
			MVector adjustedVert(
				outputVerts[i][0] * scaleAdjust[0],
				outputVerts[i][1] * scaleAdjust[1],
				outputVerts[i][2] * scaleAdjust[2]
			);

			if (isRotated)
			{
				adjustedVert = adjustedVert * rotMatrix;
			}

			outputVerts[i][0] = adjustedVert.x + positionAdjust[0];
			outputVerts[i][1] = adjustedVert.y + positionAdjust[1];
			outputVerts[i][2] = adjustedVert.z + positionAdjust[2];
		}
	}


	// Draw Lines
	if (data->fWireVisibility > 0.0f)
	{
		for (i = 0; i < 256; i++)
		{
			data->fShapeLineList[i].clear();
		}

		for (i = 0; i < edgeCount; i++)
		{
			//data->fShapeLineList[i].clear();
			data->fShapeLineList[i].append(
				outputVerts[outputEdges[i][0]][0],
				outputVerts[outputEdges[i][0]][1],
				outputVerts[outputEdges[i][0]][2]);
			data->fShapeLineList[i].append(
				outputVerts[outputEdges[i][1]][0],
				outputVerts[outputEdges[i][1]][1],
				outputVerts[outputEdges[i][1]][2]);

		}
	}

	// Draw triangles
	if (data->fSolidVisibility > 0.0f)
	{
		data->fShapeTriangleList.clear();
		for (i = 0; i < faceCount; i++)
		{
			data->fShapeTriangleList.append(
				outputVerts[outputFaces[i][0]][0],
				outputVerts[outputFaces[i][0]][1],
				outputVerts[outputFaces[i][0]][2]);
			data->fShapeTriangleList.append(
				outputVerts[outputFaces[i][1]][0],
				outputVerts[outputFaces[i][1]][1],
				outputVerts[outputFaces[i][1]][2]);
			data->fShapeTriangleList.append(
				outputVerts[outputFaces[i][2]][0],
				outputVerts[outputFaces[i][2]][1],
				outputVerts[outputFaces[i][2]][2]);
		}
	}

	// get correct color based on the state of object, e.g. active or dormant
	data->fColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

	return data;
}

// addUIDrawables() provides access to the MUIDrawManager, which can be used
// to queue up operations for drawing simple UI elements such as lines, circles and
// text. To enable addUIDrawables(), override hasUIDrawables() and make it return true.
void baconShapeBoneDrawOverride::addUIDrawables(
	const MDagPath& objPath,
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext,
	const MUserData* data)
{
	// Get data cached by prepareForDraw() for each drawable instance, then MUIDrawManager 
	// can draw simple UI by these data.
	baconShapeBoneData* pLocatorData = (baconShapeBoneData*)data;
	if (!pLocatorData)
	{
		return;
	}

	drawManager.beginDrawable();
	if (pLocatorData->fXRay)
		drawManager.beginDrawInXray();

	drawManager.setLineWidth(pLocatorData->fWireThickness);

	//drawManager.beginDrawInXray();

	//float fWireVisibility = getWireVisibility(objPath);
	//float fSolidVisibility = getSolidVisibility(objPath);

	// Draw the foot print solid/wireframe
	//drawManager.setColor(pLocatorData->fColor);
	//drawManager.setColor(MColor(0.1f, 0.8f, 0.8f, 0.2f));

	// displayStatus can tell us if it's selected
	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(objPath);


	MColor locatorColor = pLocatorData->fColor;

	if (pLocatorData->fWireVisibility > 0.0)
	{
		if (pLocatorData->fpresto == true)
		{
			if (displayStatus == MHWRender::kLead || displayStatus == MHWRender::kActive)
			{
				locatorColor.a = pLocatorData->fWireVisibility; // 0.3f;
			}
			else
			{
				locatorColor.a = 0.0f; //pLocatorData->fWireVisibility; // 1.0f;
			}
		}
		else
		{
			locatorColor.a = pLocatorData->fWireVisibility; // 1.0f;
		}

		drawManager.setColor(locatorColor);
		unsigned int i;
		unsigned int edgeCount = sizeof(pLocatorData->fShapeLineList) / sizeof(pLocatorData->fShapeLineList[0]);
		for (i = 0; i < edgeCount; i++)
		{
			drawManager.mesh(MHWRender::MUIDrawManager::kLines, pLocatorData->fShapeLineList[i]);
		}
	}


	if (pLocatorData->fSolidVisibility > 0.0 || pLocatorData->fpresto == true)
	{
		if (pLocatorData->fpresto == true)
		{
			if (displayStatus == MHWRender::kLead || displayStatus == MHWRender::kActive)
			{
				locatorColor.a = pLocatorData->fSolidVisibility; //0.08f;
			}
			else
			{
				locatorColor.a = 0.0f; //pLocatorData->fSolidVisibility; // 1.0f;
			}
		}
		else
		{
			locatorColor.a = pLocatorData->fSolidVisibility; // 0.1f;
		}
		drawManager.setColor(locatorColor);
		//drawManager.setDepthPriority(0);

		if (frameContext.getDisplayStyle() & MHWRender::MFrameContext::kGouraudShaded) {
			drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, pLocatorData->fShapeTriangleList);
		}
	}

	// Draw Text
	/*
	MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
	MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color
	drawManager.setColor( textColor );
	drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
	drawManager.text( pos,  MString("Over name"), MHWRender::MUIDrawManager::kCenter );
	*/

	//drawManager.endDrawInXray();
	if (pLocatorData->fXRay)
		drawManager.endDrawInXray();
	
	drawManager.setLineWidth(1.0);
	
	drawManager.endDrawable();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Plugin Registration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

MStatus baconShapeBone::initialize()
{

	MFnCompoundAttribute	compAttr; 
	MFnUnitAttribute		unitFn;
	MFnAttribute			attrFn;
	MFnStringData			stringFn;
	MFnEnumAttribute		enumAttr;
	MFnNumericAttribute		nAttr;
	MStatus					stat;


	// DO NOT remove Inherited Attrs from UI for a bone
	MObject inheretedAttrs[] =
	{
		baconShapeBone::localPositionX,
		baconShapeBone::localPositionY,
		baconShapeBone::localPositionZ,
		baconShapeBone::localScaleX,
		baconShapeBone::localScaleY,
		baconShapeBone::localScaleZ
	};
	unsigned int i;
	for (i = 0; i < 6; ++i)
	{
		MFnAttribute inheritedFn(inheretedAttrs[i]);
		inheritedFn.setHidden(true);
		inheritedFn.setConnectable(false);
	}

	/*
	// Size
	size = unitFn.create("size", "sz", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setKeyable(true);
	unitFn.setStorable(true);
	stat = addAttribute(size);
	if (!stat) {
		stat.perror("addAttribute");
		return stat;
	}
	*/

	// shapeType
	shapeType = enumAttr.create("shapeType", "st", 0);
	enumAttr.addField("Box", 0);
	enumAttr.addField("Capsule", 1);
	enumAttr.addField("Cylinder", 2);
	enumAttr.addField("Peg", 3);
	enumAttr.addField("Sphere", 4);
	enumAttr.addField("Circle", 5);
	enumAttr.addField("Square", 6);
	enumAttr.addField("Locator", 7);
	enumAttr.addField("Loop", 8);
	enumAttr.addField("Disc", 9);
	enumAttr.addField("Ring", 10);
	enumAttr.addField("Plus", 11);
	enumAttr.addField("Chevron", 12);
	enumAttr.addField("Four Ways", 13);
	enumAttr.addField("Arrowhead", 14);
	enumAttr.addField("Diamond", 15);
	enumAttr.addField("Heart", 16);
	enumAttr.addField("Pin", 17);
	enumAttr.addField("Marionette Part", 18);
	enumAttr.addField("Goggles", 19);
	enumAttr.addField("Shoulder Pad", 20);
	enumAttr.addField("Feather", 21);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(shapeType);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	// marionette part
	marionetteType = enumAttr.create("marionetteType", "marty", 0);
	enumAttr.addField("CS Head", 0);
	enumAttr.addField("CS Pelvis", 1);
	enumAttr.addField("CS Spine", 2);
	enumAttr.addField("CS Clavicle", 3);
	enumAttr.addField("CS UpperArm", 4);
	enumAttr.addField("CS Forearm", 5);
	enumAttr.addField("CS Hand", 6);
	enumAttr.addField("CS Thigh", 7);
	enumAttr.addField("CS Calf", 8);
	enumAttr.addField("CS Foot", 9);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(marionetteType);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	// boneType
	boneType = enumAttr.create("boneType", "bType", 0);
	enumAttr.addField("Skeleton", 0);
	enumAttr.addField("Control Rig", 1);
	enumAttr.addField("Construction", 2);
	enumAttr.addField("Ignore", 3);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(boneType);
	if (!stat) { stat.perror("addAttribute"); return stat; }


	// Wire Thickness
	wireThickness = unitFn.create("wireThickness", "wt", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	stat = addAttribute(wireThickness);
	if (!stat) {
		stat.perror("addAttribute");
		return stat;
	}

	// Wire Visibility
	wireVisibility = unitFn.create("wireVisibility", "wv", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	stat = addAttribute(wireVisibility);
	if (!stat) {
		stat.perror("addAttribute");
		return stat;
	}
	
	// Solid Visibility
	solidVisibility = unitFn.create("solidVisibility", "sv", MFnUnitAttribute::kDistance);
	unitFn.setDefault(0.05);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	stat = addAttribute(solidVisibility);
	if (!stat) {
		stat.perror("addAttribute");
		return stat;
	}

	// Presto Select
	prestoMode = nAttr.create("prestoMode", "presto", MFnNumericData::kBoolean);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setReadable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(prestoMode);
	if (!stat) { stat.perror("addAttribute"); return stat; }


	/*
	// pickerType
	pickerType = enumAttr.create("pickerType", "pickType", 0);
	enumAttr.addField("None", 0);
	enumAttr.addField("FK", 1);
	enumAttr.addField("IK", 2);
	enumAttr.setHidden(false);
	enumAttr.setKeyable(true);
	stat = addAttribute(pickerType);
	if (!stat) { stat.perror("addAttribute"); return stat; }
	*/

	// mirror
	mirror = nAttr.create("mirror", "mir", MFnNumericData::kBoolean);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setReadable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(mirror);
	if (!stat) { stat.perror("addAttribute"); return stat; }


	// xRay
	xRay = nAttr.create("xRay", "xr", MFnNumericData::kBoolean);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setReadable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(xRay);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	//--------------------------------------
	// legacy compatibility
	// Local Rotation X
	localRotationX = unitFn.create("localRotationX", "lr", MFnUnitAttribute::kAngle);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(true);
	unitFn.setStorable(true);
	unitFn.setConnectable(false);
	stat = addAttribute(localRotationX);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	// Local Rotation Y
	localRotationY = unitFn.create("localRotationY", "ly", MFnUnitAttribute::kAngle);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(true);
	unitFn.setStorable(true);
	unitFn.setConnectable(false);
	stat = addAttribute(localRotationY);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	// Local Rotation Z
	localRotationZ = unitFn.create("localRotationZ", "lz", MFnUnitAttribute::kAngle);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(true);
	unitFn.setStorable(true);
	unitFn.setConnectable(false);
	stat = addAttribute(localRotationZ);
	if (!stat) { stat.perror("addAttribute"); return stat; }


	//--------------------------------------
	// Finally replacing inhereted baloney
	// OFFSET
	// Shape Offset X
	shapeOffsetX = unitFn.create("shapeOffsetX", "offx", MFnUnitAttribute::kDistance);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeOffsetX);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	// Shape Offset Y
	shapeOffsetY = unitFn.create("shapeOffsetY", "offy", MFnUnitAttribute::kDistance);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeOffsetY);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	// Shape Offset Z
	shapeOffsetZ = unitFn.create("shapeOffsetZ", "offz", MFnUnitAttribute::kDistance);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeOffsetZ);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	shapeOffset = nAttr.create("shapeOffset", "off", shapeOffsetX, shapeOffsetY, shapeOffsetZ);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	stat = addAttribute(shapeOffset);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	//--------------------------------------
	// ORIENTATION
	// Shape Orientation X
	shapeOrientationX = unitFn.create("shapeOrientationX", "sox", MFnUnitAttribute::kAngle);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeOrientationX);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	// Shape Orientation Y
	shapeOrientationY = unitFn.create("shapeOrientationY", "soy", MFnUnitAttribute::kAngle);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeOrientationY);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	// Shape Orientation Z
	shapeOrientationZ = unitFn.create("shapeOrientationZ", "soz", MFnUnitAttribute::kAngle);
	unitFn.setDefault(0.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeOrientationZ);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	shapeOrientation = nAttr.create("shapeOrientation", "ori", shapeOrientationX, shapeOrientationY, shapeOrientationZ);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	stat = addAttribute(shapeOrientation);
	if (!stat) { stat.perror("addAttribute"); return stat; }



	//--------------------------------------
	// SIZE
	// Shape Size X
	shapeSizeX = unitFn.create("shapeSizeX", "sizex", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeSizeX);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	// Shape Size Y
	shapeSizeY = unitFn.create("shapeSizeY", "sizey", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeSizeY);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	// Shape Size Z
	shapeSizeZ = unitFn.create("shapeSizeZ", "sizez", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	//stat = addAttribute(shapeSizeZ);
	//if (!stat) { stat.perror("addAttribute"); return stat; }

	shapeSize = nAttr.create("shapeSize", "siz", shapeSizeX, shapeSizeY, shapeSizeZ);
	unitFn.setKeyable(true);
	unitFn.setHidden(false);
	unitFn.setStorable(true);
	stat = addAttribute(shapeSize);
	if (!stat) { stat.perror("addAttribute"); return stat; }


	/*

	// draw last
	aEnableDrawLast = nAttr.create("drawLast", "dl", MFnNumericData::kBoolean);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setReadable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(aEnableDrawLast);
	if (!stat) { stat.perror("addAttribute"); return stat; }


	aTransparency = nAttr.create("transparency", "t", MFnNumericData::kFloat);
	nAttr.setDefault(0.5);
	nAttr.setKeyable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(aTransparency);
	if (!stat) { stat.perror("addAttribute"); return stat; }

	aEnableTransparencySort = nAttr.create("transparencySort", "ts", MFnNumericData::kBoolean);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	stat = addAttribute(aEnableTransparencySort);
	if (!stat) { stat.perror("addAttribute"); return stat; }
	*/

	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, "Bacon-Strip.com", "1.0", "Any");

	status = plugin.registerNode(
		"baconShapeBone",
		baconShapeBone::id,
		&baconShapeBone::creator,
		&baconShapeBone::initialize,
		MPxNode::kLocatorNode,
		sUseLegacyDraw ? NULL : &baconShapeBone::drawDbClassification);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	if (!sUseLegacyDraw)
	{
		status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
			baconShapeBone::drawDbClassification,
			baconShapeBone::drawRegistrantId,
			baconShapeBoneDrawOverride::Creator);
		if (!status) {
			status.perror("registerDrawOverrideCreator");
			return status;
		}
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	if (!sUseLegacyDraw)
	{
		status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
			baconShapeBone::drawDbClassification,
			baconShapeBone::drawRegistrantId);
		if (!status) {
			status.perror("deregisterDrawOverrideCreator");
			return status;
		}
	}

	status = plugin.deregisterNode(baconShapeBone::id);
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	return status;
}
