"""
// ==========================================================================
// www.Bacon-Strip.com
// Luis Alonso
// Creative Commons 2017
// Bacon-Strip Node ID Block are: 0x0012a940 - 0x0012aa3f
// Registered for Bacon-Strip [Luis Alonso]
//
// ==========================================================================

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// Produces the dependency graph node "baconViseme".
// This plug-in is part of a rigging package known as The "Bacon-Strip"
//
// ID: 
// 0x0012aa02
//
////////////////////////////////////////////////////////////////////////
"""


import math, sys
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
kPluginNodeTypeName = "baconViseme"
baconVisemeId = OpenMaya.MTypeId(0x0012aa02)


class baconViseme(OpenMayaMPx.MPxNode):
	# INPUTS
	visemeScale = OpenMaya.MObject()
	xComponent = OpenMaya.MObject()
	yComponent = OpenMaya.MObject()
	# OUTPUTS
	Up = OpenMaya.MObject()
	Down = OpenMaya.MObject()
	Left = OpenMaya.MObject()
	Right = OpenMaya.MObject()
	
	def __init__(self):
		OpenMayaMPx.MPxNode.__init__(self)

	def compute(self, plug, dataBlock):
		if plug == self.Up or self.Down or self.Left or self.Right:
			visemeScaleHandle = dataBlock.inputValue(baconViseme.visemeScale)
			vScale = visemeScaleHandle.asFloat()

			xComponentHandle = dataBlock.inputValue(baconViseme.xComponent)
			vX = xComponentHandle.asFloat()

			yComponentHandle = dataBlock.inputValue(baconViseme.yComponent)
			vY = yComponentHandle.asFloat()
			
			def nClamp(n): 
				return (max(0.0, min(n, 1.0)))
			
			# CALCULATION
			UpValue = nClamp( vY / vScale )
			DownValue = nClamp( vY / (vScale * -1.0) )
			RightValue = nClamp( vX / vScale)
			LeftValue = nClamp( vX / (vScale * -1.0) )
			
			#OUTPUT
			UpHandle = dataBlock.outputValue( self.Up )
			UpHandle.setFloat(UpValue)
			UpHandle.setClean()

			DownHandle = dataBlock.outputValue( self.Down )
			DownHandle.setFloat(DownValue)
			DownHandle.setClean()

			LeftHandle = dataBlock.outputValue( self.Left )
			LeftHandle.setFloat(LeftValue)
			LeftHandle.setClean()
			
			RightHandle = dataBlock.outputValue( self.Right )
			RightHandle.setFloat(RightValue)
			RightHandle.setClean()


def nodeCreator():
	return OpenMayaMPx.asMPxPtr( baconViseme() )

def nodeInitializer():
	self = baconViseme
	unitAttr = OpenMaya.MFnUnitAttribute()
	typedAttr = OpenMaya.MFnTypedAttribute()
	enumAttr = OpenMaya.MFnEnumAttribute()
	numAttr = OpenMaya.MFnNumericAttribute()


	# Input visemeScale
	visemeScaleAttr = OpenMaya.MFnNumericAttribute()
	self.visemeScale = visemeScaleAttr.create("visemeScale", "vlen", OpenMaya.MFnNumericData.kFloat, 1.0)
	visemeScaleAttr.setStorable(True)
	visemeScaleAttr.setKeyable(True)
	self.addAttribute( self.visemeScale )

	# Input xComponent
	xComponentAttr = OpenMaya.MFnNumericAttribute()
	self.xComponent = xComponentAttr.create("xComponent", "xc", OpenMaya.MFnNumericData.kFloat, 0.0)
	xComponentAttr.setStorable(True)
	xComponentAttr.setKeyable(True)
	self.addAttribute( self.xComponent )

	# Input yComponent
	yComponentAttr = OpenMaya.MFnNumericAttribute()
	self.yComponent = yComponentAttr.create("yComponent", "yc", OpenMaya.MFnNumericData.kFloat, 0.0)
	yComponentAttr.setStorable(True)
	yComponentAttr.setKeyable(True)
	self.addAttribute( self.yComponent )


	# Output Up
	UpAttr = OpenMaya.MFnNumericAttribute()
	self.Up = UpAttr.create("Up", "up", OpenMaya.MFnNumericData.kFloat, 0.0)
	UpAttr.setWritable(False)
	self.addAttribute( self.Up )

	# Output Down
	DownAttr = OpenMaya.MFnNumericAttribute()
	self.Down = DownAttr.create("Down", "dn", OpenMaya.MFnNumericData.kFloat, 0.0)
	DownAttr.setWritable(False)
	self.addAttribute( self.Down )

	# Output Left
	LeftAttr = OpenMaya.MFnNumericAttribute()
	self.Left = LeftAttr.create("Left", "lt", OpenMaya.MFnNumericData.kFloat, 0.0)
	LeftAttr.setWritable(False)
	self.addAttribute( self.Left )

	# Output Right
	RightAttr = OpenMaya.MFnNumericAttribute()
	self.Right = RightAttr.create("Right", "rt", OpenMaya.MFnNumericData.kFloat, 0.0)
	RightAttr.setWritable(False)
	self.addAttribute( self.Right )


	def AffectsMany(instAttr):
		self.attributeAffects( self.visemeScale, instAttr)
		self.attributeAffects( self.xComponent, instAttr)
		self.attributeAffects( self.yComponent, instAttr)

	AffectsMany( self.Up )
	AffectsMany( self.Down )
	AffectsMany( self.Left )
	AffectsMany( self.Right )


# initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.registerNode( kPluginNodeTypeName, baconVisemeId, nodeCreator, nodeInitializer )
	except:
		sys.stderr.write( "Failed to register node: %s" % kPluginNodeTypeName )
		raise


# uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.deregisterNode( baconVisemeId )
	except:
		sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeTypeName )
		raise
