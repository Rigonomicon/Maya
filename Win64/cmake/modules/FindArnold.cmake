# -Find Arnold module
#
# Variables that will be defined:
# ARNOLD_PATH          Path to install Arnold SDK
# ARNOLD_INCLUDE_DIR   Path to the Arnold's include directory
# ARNOLD_BIN_DIR       Path to the Arnold's bin directory
#

# set ARNOLD_PATH 
# set(ARNOLD_PATH "")

if(NOT DEFINED ENV{ARNOLD_PATH} AND NOT DEFINED ARNOLD_PATH)
  message("Need to install Arnold SDK and set environment variable ARNOLD_PATH as the installation path!")
else()
  find_path(ARNOLD_INCLUDE_DIR NAMES ai.h
  PATHS
    ${ARNOLD_PATH}
    $ENV{ARNOLD_PATH}
  PATH_SUFFIXES
    "include/"
  DOC "Arnold include path"
)
  
  if (WIN32)
    set(ARNOLD_BIN_SUFFIX "lib")
	set(AILIB "ai.lib")
  elseif (APPLE)
    set(ARNOLD_BIN_SUFFIX "bin")
	set(AILIB "libai.dylib")
  else()
    set(ARNOLD_BIN_SUFFIX "bin")
	set(AILIB "libai.so")
  endif()
  
  find_path(ARNOLD_BIN_DIR ${AILIB}
  PATHS
    ${ARNOLD_PATH}
    $ENV{ARNOLD_PATH}
  PATH_SUFFIXES
    "${ARNOLD_BIN_SUFFIX}/"
  DOC "Arnold lib path"
)
  find_library(AI_LIB NAMES ${AILIB} PATHS ${ARNOLD_BIN_DIR})
  set(ARNOLD_LIBRARY ${AI_LIB})
endif()

