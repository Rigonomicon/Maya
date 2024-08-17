# - Find DirectXSDK location

#[=======================================================================[.rst:
FindDirectXSDK
---------------

Find the DirectX SDK

IMPORTED Targets
^^^^^^^^^^^^^^^^

For Windows, user needs to install Microsoft DirectX SDK and set up the library and include path

This module is incomplete, it defines :prop_tgt:`IMPORTED` target ``DirectXSDK::DirectXSDK`` and some
libraries that are part of it, if found. Some specific subprojects are not defined and would need to
be added to this file if needed.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

::

  ``DIRECTXSDK_FOUND``
  True if the DirectX SDK headers and libraries were found
  ``DIRECTXSDK_INCLUDE_DIR``
  the directory containing the DirectX SDK headers
  ``DIRECTXSDK_LIBRARY_DIR``
  the directory containing the DirectX SDK libraries
  ``DIRECTXSDK_LIBRARIES``
  list of defined libraries.

Hints
^^^^^
  ``DIRECTXSDK_ROOT``
  path to the DirectX SDK library install root

  A user may set ``DIRECTXSDK_ROOT`` to a DirectX SDK installation root to tell this
  module where to look.
#]=======================================================================]

# set(DIRECTXSDK_ROOT "")

if(TARGET DirectXSDK::DirectXSDK)
	set(DIRECTXSDK_FOUND ON)
	get_target_property(DIRECTXSDK_INCLUDE_DIR DirectXSDK::DirectXSDK INTERFACE_INCLUDE_DIRECTORIES)
	get_target_property(DIRECTXSDK_LIBRARIES DirectXSDK::DirectXSDK INTERFACE_LINK_LIBRARIES)
endif()

if(DIRECTXSDK_FOUND)
	message(STATUS "FindDirectXSDK: DIRECTXSDK_FOUND is set, skipping search.")
	return()
endif()

if(NOT WIN32)
	return()
endif()

if(NOT DEFINED ENV{DIRECTXSDK_ROOT} AND NOT DEFINED DIRECTXSDK_ROOT)
	message(WARNING "Need to install the DirectX SDK and set environment or cmake variable DIRECTXSDK_ROOT to the installation path!")
	return()
endif()


set(DIRECTXSDK_SEARCH_LIST "")

if(DEFINED ENV{DIRECTXSDK_ROOT})
	list(APPEND DIRECTXSDK_SEARCH_LIST "$ENV{DIRECTXSDK_ROOT}")
endif()

if(DEFINED DIRECTXSDK_ROOT)
	list(APPEND DIRECTXSDK_SEARCH_LIST "${DIRECTXSDK_ROOT}")
endif()

find_library(DIRECTXSDK_LIBRARY_DIR NAMES d3dx11
	HINTS
		${DIRECTXSDK_SEARCH_LIST}
	PATH_SUFFIXES
		/Lib/x64
		/Lib
	DOC
		"DirectX SDK's lib path"
	)

find_path(DIRECTXSDK_INCLUDE_DIR D3DX11.h
	HINTS
		${DIRECTXSDK_SEARCH_LIST}
	PATH_SUFFIXES
		/Include
		/include
	DOC
		"DirectX SDK's include path"
	 )

if(DIRECTXSDK_LIBRARY_DIR AND DIRECTXSDK_INCLUDE_DIR)
	get_filename_component(DIRECTXSDK_LIBRARY_EXT ${DIRECTXSDK_LIBRARY_DIR} EXT)
	get_filename_component(DIRECTXSDK_LIBRARY_DIR ${DIRECTXSDK_LIBRARY_DIR} DIRECTORY)

	set(DIRECTXSDK_FOUND ON)
	set(DIRECTXSDK_LIBRARIES "")
endif()


if(DIRECTXSDK_FOUND)
	set(DIRECTXSDK_LIBNAMES
		d3d9
		d3d11
		dxguid
		D3DCompiler
		)

	# these libs come with debug versions
	set(DIRECTXSDK_LIBNAMES2
		D3DCSX
		d3dx9
		d3dx10
		d3dx11
		)

	if(NOT TARGET DirectXSDK::DirectXSDK)
		foreach(lib ${DIRECTXSDK_LIBNAMES})
			add_library(DirectXSDK::${lib} UNKNOWN IMPORTED GLOBAL)
			set_target_properties(DirectXSDK::${lib} PROPERTIES
				IMPORTED_LOCATION             "${DIRECTXSDK_LIBRARY_DIR}/${lib}${DIRECTXSDK_LIBRARY_EXT}"
				INTERFACE_INCLUDE_DIRECTORIES "${DIRECTXSDK_INCLUDE_DIR}")

			list(APPEND DIRECTXSDK_LIBRARIES DirectXSDK::${lib})
		endforeach()

		foreach(lib ${DIRECTXSDK_LIBNAMES2})
			add_library(DirectXSDK::${lib} UNKNOWN IMPORTED GLOBAL)
			set_target_properties(DirectXSDK::${lib} PROPERTIES
				IMPORTED_LOCATION             "${DIRECTXSDK_LIBRARY_DIR}/${lib}${DIRECTXSDK_LIBRARY_EXT}"
				INTERFACE_INCLUDE_DIRECTORIES "${DIRECTXSDK_INCLUDE_DIR}")

			set_property(TARGET DirectXSDK::${lib} PROPERTY DBG_POSTFIX d)

			list(APPEND DIRECTXSDK_LIBRARIES DirectXSDK::${lib})
		endforeach()

		# top level target
		add_library(DirectXSDK::DirectXSDK INTERFACE IMPORTED GLOBAL)
		set_property(TARGET DirectXSDK::DirectXSDK PROPERTY
			INTERFACE_INCLUDE_DIRECTORIES ${DIRECTXSDK_INCLUDE_DIR})
		set_property(TARGET DirectXSDK::DirectXSDK PROPERTY
			INTERFACE_LINK_LIBRARIES
				${DIRECTXSDK_LIBRARIES})
	endif()

	mark_as_advanced(DIRECTXSDK_FOUND)
endif()
