# - Find Alembic module
#
# Variables that will be defined:
# AlEMBIC_INCLUDE_DIR    Path to the devkit's include directories
# ALEMBIC_LIBRARIES      All the Alembic libraries
#

if (NOT DEFINED ENV{DEVKIT_LOCATION})
	message("Please set DEVKIT_LOCATION environment variable to the folder of your devkit package.")
else ()

	# Alembic location directory
	find_path(ALEMBIC_DIR NAMES devkit/Alembic
		PATHS
		$ENV{DEVKIT_LOCATION}
		${MAYA_DEVKIT_BASE_DIR}
    DOC "devkit Alembic path"
	)
	
	set(ALEMBIC_DIR "${ALEMBIC_DIR}/devkit/Alembic")
 
	# Alembic include directory
	set(ALEMBIC_LIBRARY_DIR ${ALEMBIC_DIR}/lib)
	set(ALEMBIC_INCLUDE ${ALEMBIC_DIR}/include)
	set(IMATH_INCLUDE ${ALEMBIC_INCLUDE}/Imath)

	set(ALEMBIC_INCLUDE_DIRS ${ALEMBIC_INCLUDE} ${IMATH_INCLUDE})
	
	set(ALEMBIC_FOUND FALSE)
	# Alembic libraries
	file(GLOB Imath_lib_path "${ALEMBIC_LIBRARY_DIR}/*Imath-*" )
	get_filename_component(Imath_lib_name ${Imath_lib_path} NAME)
	set(_ALEMBIC_LIBRARIES Alembic ${Imath_lib_name})

	foreach(ALEMBIC_LIBRARY ${_ALEMBIC_LIBRARIES})
		string(TOUPPER ${ALEMBIC_LIBRARY} ALEMBIC_UPPER)
		find_library(${ALEMBIC_UPPER}_LIB NAMES ${ALEMBIC_LIBRARY} PATHS ${ALEMBIC_LIBRARY_DIR}
			NO_DEFAULT_PATH)
		if (${ALEMBIC_UPPER}_LIB AND EXISTS ${${ALEMBIC_UPPER}_LIB})
			set(ALEMBIC_LIBRARIES ${ALEMBIC_LIBRARIES} ${${ALEMBIC_UPPER}_LIB})
			set(ALEMBIC_FOUND TRUE)
		else()
			set(MISSING_ALEMBIC_LIBS ${MISSING_ALEMBIC_LIBS} ${ALEMBIC_LIBRARY}) 
		endif()
	endforeach(ALEMBIC_LIBRARY)
  
	list(LENGTH MISSING_ALEMBIC_LIBS MISSING_LENGTH)
	if (MISSING_LENGTH GREATER 0)
		message("Missing Alembic libraries: " ${MISSING_ALEMBIC_LIBS})
	endif()
endif()