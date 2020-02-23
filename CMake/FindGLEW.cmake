# Find file for GLEW

# Find includes
set(header GL/glew.h)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(GLEW_INCLUDE_DIR
  NAMES
	${header}
  PATHS
	${INCLUDE_SEARCH_DIR}
  NO_DEFAULT_PATH)
set(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIR})


# Find libraries
set(debug_postfix d.lib)
set(release_postfix .lib)
set(LIB_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/lib)

find_library(GLEW_LIBRARY-RELEASE
	NAMES
		glew${release_postfix}
		libglew${release_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)

find_library(GLEW_LIBRARY-DEBUG
	NAMES
		glew${debug_postfix}
		libglew${debug_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Debug
	NO_DEFAULT_PATH)

if (EXISTS ${GLEW_LIBRARY-RELEASE})
	list(APPEND GLEW_LIBRARIES optimized ${GLEW_LIBRARY-RELEASE})
	list(APPEND GLEW_RELEASE_LIBRARIES ${GLEW_LIBRARY-RELEASE})
endif()
mark_as_advanced(GLEW_LIBRARY-RELEASE)

if (EXISTS ${GLEW_LIBRARY-DEBUG})
	list(APPEND GLEW_LIBRARIES debug ${GLEW_LIBRARY-DEBUG})
	list(APPEND GLEW_DEBUG_LIBRARIES ${GLEW_LIBRARY-DEBUG})
endif()
mark_as_advanced(GLEW_LIBRARY-DEBUG)


# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG
  GLEW_INCLUDE_DIR)