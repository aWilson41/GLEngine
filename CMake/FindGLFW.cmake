# Find file for GLFW

# Find includes
set(header GLFW/glfw3.h)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)
find_path(GLFW_INCLUDE_DIR
  NAMES
	${header}
  PATHS
	${INCLUDE_SEARCH_DIR}
  NO_DEFAULT_PATH)


# Find libraries
set(LIB_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/lib)
find_library(GLFW_LIBRARY-RELEASE
	NAMES
		glfw3.lib
		libglfw3.lib
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)
find_library(GLFW_LIBRARY-DEBUG
	NAMES
		glfw3.lib
		libglfw3.lib
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Debug
	NO_DEFAULT_PATH)

if (EXISTS ${GLFW_LIBRARY-RELEASE})
	list(APPEND GLFW_LIBRARIES optimized ${GLFW_LIBRARY-RELEASE})
	list(APPEND GLFW_RELEASE_LIBRARIES ${GLFW_LIBRARY-RELEASE})
endif()
mark_as_advanced(GLFW_LIBRARY-RELEASE)

if (EXISTS ${GLFW_LIBRARY-DEBUG})
	list(APPEND GLFW_LIBRARIES debug ${GLFW_LIBRARY-DEBUG})
	list(APPEND GLFW_DEBUG_LIBRARIES ${GLFW_LIBRARY-DEBUG})
endif()
mark_as_advanced(GLFW_LIBRARY-DEBUG)


# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG
	GLFW_INCLUDE_DIR)