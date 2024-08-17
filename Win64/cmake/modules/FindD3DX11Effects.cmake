# - Find D3DX11Effects location

#[=======================================================================[.rst:
FindD3DXEffects
---------------

Find the D3DXEffects library (when not part of a DirectX SDK install)

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``DirectXSDK::D3DX11Effects``, if
D3DX11Effects has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

::

  ``D3DX11EFFECTS_FOUND``
  True if the D3DX11Effects headers and libraries were found
  ``DIRECTXSDK_D3DX11EFFECTS_FOUND``
  True if the libraries were found on the DirectX SDK Samples folder.
  ``D3DX11EFFECTS_INCLUDE_DIR``
  the directory containing the D3DX11Effects headers
  ``D3DX11EFFECTS_LIBRARY_DIR``
  the directory containing the D3DX11Effects library
  ``D3DX11EFFECTS_LIBRARIES``
  D3DX11Effects libraries to be linked

Hints
^^^^^
  ``D3DX11EFFECTS_ROOT``
  path to the D3DX11Effects library install root
  ``D3DX11EFFECTS_LIBRARY``
  path to the D3DX11Effects library

  A user may set ``D3DX11EFFECTS_ROOT`` to a D3DXEffects installation root to tell this
  module where to look.
#]=======================================================================]

# set(D3DX11EFFECTS_ROOT "")

if(TARGET DirectXSDK::D3DX11Effects)
	set(D3DX11EFFECTS_FOUND ON)
	get_target_property(D3DX11EFFECTS_INCLUDE_DIR DirectXSDK::D3DX11Effects INTERFACE_INCLUDE_DIRECTORIES)
	get_target_property(D3DX11EFFECTS_LIBRARIES DirectXSDK::D3DX11Effects IMPORTED_IMPLIB)
endif()


if(D3DX11EFFECTS_FOUND)
	message(STATUS "FindD3DX11Effects: D3DX11EFFECTS_FOUND is set, skipping search.")
	return()
endif()

if(NOT WIN32)
	return()
endif()

if(NOT DEFINED ENV{D3DX11EFFECTS_ROOT} AND NOT DEFINED D3DX11EFFECTS_ROOT)
	message(WARNING "FindD3DX11Effects: D3DX11EFFECTS_ROOT variable not found, it needs to be set as an environment variable or cmake variable to attempt to find the D3DX11Effects library.")
	message(STATUS "FindD3DX11Effects: Will attempt to find the DirectX SDK build instead.")

	if(NOT DEFINED ENV{DIRECTXSDK_ROOT} AND NOT DEFINED DIRECTXSDK_ROOT)
		message(WARNING "FindD3DX11Effects: DIRECTXSDK_ROOT variable not found either, it needs to be set as an environment variable or cmake variable to attempt to find the D3DX11Effects library.")
		message(WARNING "FindD3DX11Effects: Library not found.")
		return()
	endif()
endif()


# locations already hinted, move on
if(D3DX11EFFECTS_LIBRARY AND D3DX11EFFECTS_INCLUDE_DIR)
	set(D3DX11EFFECTS_FOUND ON)
	set(D3DX11EFFECTS_LIBRARY_DIR "${D3DX11EFFECTS_LIBRARY}")
endif()


# search for library
if(NOT D3DX11EFFECTS_FOUND)
	set(D3DX11EFFECTS_SEARCH_LIST "")

	if(DEFINED D3DX11EFFECTS_LIBRARY)
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "${D3DX11EFFECTS_LIBRARY}")
	endif()

	if(DEFINED ENV{D3DX11EFFECTS_ROOT})
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "$ENV{D3DX11EFFECTS_ROOT}")
	endif()

	if(DEFINED D3DX11EFFECTS_ROOT)
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "${D3DX11EFFECTS_ROOT}")
	endif()

	find_library(D3DX11EFFECTS_LIBRARY_DIR NAMES D3DX11Effects
		HINTS
			${D3DX11EFFECTS_SEARCH_LIST}
		PATH_SUFFIXES
			/lib
		DOC
			"D3DX11Effects's lib path"
		)


	# search for headers
	set(D3DX11EFFECTS_SEARCH_LIST "")

	if(DEFINED ENV{D3DX11EFFECTS_ROOT})
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "$ENV{D3DX11EFFECTS_ROOT}")
	endif()

	if(DEFINED D3DX11EFFECTS_ROOT)
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "${D3DX11EFFECTS_ROOT}")
	endif()

	find_path(D3DX11EFFECTS_INCLUDE_DIR d3dx11effect.h
		HINTS
			${D3DX11EFFECTS_SEARCH_LIST}
		PATH_SUFFIXES
			/include
			/Inc
			/include/DX11
			/inc
		DOC
			"D3DX11Effects's include path"
		)


	if(D3DX11EFFECTS_LIBRARY_DIR AND D3DX11EFFECTS_INCLUDE_DIR)
		set(D3DX11EFFECTS_FOUND ON)
	endif()
endif()


# still not found, try the DirectX SDK
if(NOT D3DX11EFFECTS_FOUND)
	unset(D3DX11EFFECTS_LIBRARY_DIR)
	unset(D3DX11EFFECTS_INCLUDE_DIR)

	set(D3DX11EFFECTS_SEARCH_LIST "")

	if(DEFINED ENV{DIRECTXSDK_ROOT})
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "$ENV{DIRECTXSDK_ROOT}")
	endif()

	if(DEFINED DIRECTXSDK_ROOT)
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "${DIRECTXSDK_ROOT}")
	endif()

	find_library(D3DX11EFFECTS_LIBRARY_DIR NAMES D3DX11Effects
		HINTS
			${D3DX11EFFECTS_SEARCH_LIST}
		PATH_SUFFIXES
			/Samples/C++/Effects11/Lib
		DOC
			"D3DX11Effects's lib path"
		)


	set(D3DX11EFFECTS_SEARCH_LIST "")

	if(DEFINED ENV{DIRECTXSDK_ROOT})
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "$ENV{DIRECTXSDK_ROOT}")
	endif()

	if(DEFINED DIRECTXSDK_ROOT)
		list(APPEND D3DX11EFFECTS_SEARCH_LIST "${DIRECTXSDK_ROOT}")
	endif()

	find_path(D3DX11EFFECTS_INCLUDE_DIR d3dx11effect.h
		HINTS
			${D3DX11EFFECTS_SEARCH_LIST}
		PATH_SUFFIXES
			/Samples/C++/Effects11/Inc
		DOC
			"D3DX11Effects's include path"
		)

	if(D3DX11EFFECTS_LIBRARY_DIR AND D3DX11EFFECTS_INCLUDE_DIR)
		set(D3DX11EFFECTS_FOUND ON)

		# use this to detect that the library found was part of the DirectX SDK tree
		set(DIRECTXSDK_D3DX11EFFECTS_FOUND ON)
	endif()
endif()

if(D3DX11EFFECTS_FOUND)
	set(D3DX11EFFECTS_LIBRARIES ${D3DX11EFFECTS_LIBRARY_DIR})

	# remove filename from variable to match docs
	get_filename_component(D3DX11EFFECTS_LIBRARY_DIR ${D3DX11EFFECTS_LIBRARY_DIR} DIRECTORY)

	if(NOT TARGET DirectXSDK::D3DX11Effects)
		add_library(DirectXSDK::D3DX11Effects UNKNOWN IMPORTED GLOBAL)
		set_target_properties(DirectXSDK::D3DX11Effects PROPERTIES
			IMPORTED_LOCATION             "${D3DX11EFFECTS_LIBRARIES}"
			INTERFACE_INCLUDE_DIRECTORIES "${D3DX11EFFECTS_INCLUDE_DIR}")
	endif()

	mark_as_advanced(D3DX11EFFECTS_FOUND)
	mark_as_advanced(D3DX11EFFECTS_LIBRARIES)
endif()
