#  PYSIDE_PYTHONPATH    - Path to where the PySide2 Python module files could be found
#  PYSIDE_TYPESYSTEMS   - Type system files that should be used by other bindings extending PySide2
#  PYSIDE_GLUE          - Path to module glue files.


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was PySide2Config-spec.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# Import targets only when using an installed PySide2 config file (so not during a regular
# PySide2 build, or during a super project build).
if (NOT TARGET PySide2::pyside2)
    include("${CMAKE_CURRENT_LIST_DIR}/PySide2Targets.cmake")
endif()

# Set relocatable variables.
set_and_check(PYSIDE_PYTHONPATH "${PACKAGE_PREFIX_DIR}/Lib/site-packages/PySide2")
set_and_check(PYSIDE_TYPESYSTEMS "${PACKAGE_PREFIX_DIR}/share/PySide2/typesystems")
set_and_check(PYSIDE_GLUE "${PACKAGE_PREFIX_DIR}/share/PySide2/glue")
