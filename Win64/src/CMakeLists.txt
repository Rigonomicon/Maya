find_package(Maya REQUIRED)
set(LIBRARIES
     OpenMaya
     OpenMayaAnim
     OpenMayaRender
     Foundation
     OpenMayaUI
     clew
)


###############################################################################
# bacon2BoneIK
project(bacon2BoneIK VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"bacon2BoneIK.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# bacon3BoneIK
project(bacon3BoneIK VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"bacon3BoneIK.cpp"

)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconAlign
project(baconAlign VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconAlign.cpp"

)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})




###############################################################################
# baconBlend
project(baconBlend VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconBlend.cpp"

)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})


###############################################################################
# baconDoubleBezier
project(baconDoubleBezier VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconDoubleBezier.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})


###############################################################################
# baconDualHinge
project(baconDualHinge VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconDualHinge.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})


###############################################################################
# baconLookAt
project(baconLookAt VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconLookAt.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})


###############################################################################
# baconMatrixCompose
project(baconMatrixCompose VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconMatrixCompose.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})


###############################################################################
# baconPreheat
project(baconPreheat VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconPreheat.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconRibbon
project(baconRibbon VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconRibbon.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconRoll
project(baconRoll VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconRibbon.cpp"

)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconShapeBone
project(baconShapeBone VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconShapeBoneVisuals.h"
	"baconShapeBone.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconShapeLine
project(baconShapeLine VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconShapeLine.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconShapeRibbon
project(baconShapeRibbon VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconShapeRibbon.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconSlidingPivot
project(baconSlidingPivot VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconSlidingPivot.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconTransformDecompose
project(baconTransformDecompose VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconTransformDecompose.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})

###############################################################################
# baconUVProjection
project(baconUVProjection VERSION 1.0 DESCRIPTION "Rigonomicon.com - your source for learning how to rig.")
set(SOURCE_FILES
	"baconMath.h"
	"baconUVProjection.cpp"
)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})






