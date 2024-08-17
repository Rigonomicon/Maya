

cmake_minimum_required(VERSION 3.13)
project(Rigonomicon)
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)
set(LIBRARIES
     OpenMaya
     OpenMayaAnim
     OpenMayaRender
     Foundation
     OpenMayaUI
     OpenMayaFX
     clew
)


#-----------------------------
# bacon2BoneIK
project(bacon2BoneIK)
set(PROJECT_NAME bacon2BoneIK)
set(SOURCE_FILES
   bacon2BoneIK.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# bacon3BoneIK
project(bacon3BoneIK)
set(PROJECT_NAME bacon3BoneIK)
set(SOURCE_FILES
   bacon3BoneIK.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconAlign
project(baconAlign)
set(PROJECT_NAME baconAlign)
set(SOURCE_FILES
   baconMath.h
   baconAlign.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconBlend
project(baconBlend)
set(PROJECT_NAME baconBlend)
set(SOURCE_FILES
   baconMath.h
   baconBlend.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconDoubleBezier
project(baconDoubleBezier)
set(PROJECT_NAME baconDoubleBezier)
set(SOURCE_FILES
   baconDoubleBezier.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconDualHinge
project(baconDualHinge)
set(PROJECT_NAME baconDualHinge)
set(SOURCE_FILES
   baconMath.h
   baconDualHinge.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconLookAt
project(baconLookAt)
set(PROJECT_NAME baconLookAt)
set(SOURCE_FILES
   baconMath.h
   baconLookAt.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconMatrixCompose
project(baconMatrixCompose)
set(PROJECT_NAME baconMatrixCompose)
set(SOURCE_FILES
   baconMath.h
   baconMatrixCompose.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconPreheat
project(baconPreheat)
set(PROJECT_NAME baconPreheat)
set(SOURCE_FILES
   baconMath.h
   baconPreheat.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconRibbon
project(baconRibbon)
set(PROJECT_NAME baconRibbon)
set(SOURCE_FILES
   baconMath.h
   baconRibbon.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconRoll
project(baconRoll)
set(PROJECT_NAME baconRoll)
set(SOURCE_FILES
   baconMath.h
   baconRoll.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconShapeBone
project(baconShapeBone)
set(PROJECT_NAME baconShapeBone)
set(SOURCE_FILES
   baconMath.h
   baconShapeBone.cpp
   baconShapeBoneVisuals.h
)
build_plugin()
#-----------------------------

#-----------------------------
# baconShapeLine
project(baconShapeLine)
set(PROJECT_NAME baconShapeLine)
set(SOURCE_FILES
   baconMath.h
   baconShapeLine.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconShapeRibbon
project(baconShapeRibbon)
set(PROJECT_NAME baconShapeRibbon)
set(SOURCE_FILES
   baconMath.h
   baconShapeRibbon.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconSlidingPivot
project(baconSlidingPivot)
set(PROJECT_NAME baconSlidingPivot)
set(SOURCE_FILES
   baconMath.h
   baconSlidingPivot.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconTransformDecompose
project(baconTransformDecompose)
set(PROJECT_NAME baconTransformDecompose)
set(SOURCE_FILES
   baconMath.h
   baconTransformDecompose.cpp
)
build_plugin()
#-----------------------------

#-----------------------------
# baconUVProjection
project(baconUVProjection)
set(PROJECT_NAME baconUVProjection)
set(SOURCE_FILES
   baconMath.h
   baconUVProjection.cpp
)
build_plugin()
#-----------------------------


#-----------------------------
# boneDynamicsNode
project(boneDynamicsNode)
set(SOURCE_FILES
   boneDynamicsNode.h
   boneDynamicsNode.cpp
   pluginMain.cpp
)
build_plugin()
#-----------------------------


