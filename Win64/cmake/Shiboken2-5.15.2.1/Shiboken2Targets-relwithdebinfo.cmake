#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Shiboken2::libshiboken" for configuration "RelWithDebInfo"
set_property(TARGET Shiboken2::libshiboken APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Shiboken2::libshiboken PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/shiboken2.cp310-win_amd64.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/shiboken2.cp310-win_amd64.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Shiboken2::libshiboken )
list(APPEND _IMPORT_CHECK_FILES_FOR_Shiboken2::libshiboken "${_IMPORT_PREFIX}/lib/shiboken2.cp310-win_amd64.lib" "${_IMPORT_PREFIX}/bin/shiboken2.cp310-win_amd64.dll" )

# Import target "Shiboken2::shiboken2" for configuration "RelWithDebInfo"
set_property(TARGET Shiboken2::shiboken2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Shiboken2::shiboken2 PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "S:/OSSCICD/workspace/pyside_maya/src/pyside3_install/py3.10-qt5.15.2-64bit-relwithdebinfo/bin/shiboken2.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS Shiboken2::shiboken2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Shiboken2::shiboken2 "S:/OSSCICD/workspace/pyside_maya/src/pyside3_install/py3.10-qt5.15.2-64bit-relwithdebinfo/bin/shiboken2.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
