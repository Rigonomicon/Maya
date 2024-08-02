'''
Bacon-Strip Startup
Written by Luis Alonso 2024
Startup for Bacon-Strip.

MIT License

Copyright (c) 2017 Luis Alonso

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''


#======================================================================
# Startup functions
print('Starting Bacon-Strip plugins.')
print('Bacon-Strip - MIT license - Copyright (c) 2017-2024 Luis Alonso.')

# load PyMel at startup (hopefully it's intalled). Thanks Autodesk.
# import pymel.core as pm # removed since Autodesk was petty. Major overhaul underway. Thanks Autodesk.
import maya.cmds as cmds
import maya.OpenMaya as om
import maya.api.OpenMaya as om2


#======================================================================
# pre-load bacon-strip plugins.
baconPlugins = ['baconShapeBone', 'baconShapeLine', 'baconAlign', 'baconLookAt', 'baconMatrixCompose', 'baconPreheat', 'baconRoll', 'baconSlidingPivot', \
	'bacon2BoneIK', 'bacon3BoneIK', 'baconShapeRibbon', 'baconDualHinge', 'baconDoubleBezier', 'baconBlend' ]
for plug in baconPlugins:
	if cmds.pluginInfo(plug, query=True, loaded=True) == False:
		cmds.loadPlugin(plug)

#======================================================================
# include bacon shapes to animation cache system. Thanks for gate keeping shape plugins Autodesk. Also cool.
def baconCacheUpdate(clientData=None):
	cmds.cacheEvaluator( newFilter="nodeTypes", newFilterParam="types=+baconShapeBone,+baconShapeLine,+baconShapeRibbon", newAction="enableEvaluationCache")
	#print('Rigonomicon powered by Bacon-Strip - MIT license - Copyright (c) 2017-2024 Luis Alonso.')

baconCallbackID1 = om2.MSceneMessage.addCallback( om2.MSceneMessage.kAfterOpen, baconCacheUpdate )
baconCallbackID2 = om2.MSceneMessage.addCallback( om2.MSceneMessage.kAfterNew, baconCacheUpdate )

 
