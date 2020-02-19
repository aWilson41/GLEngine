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
set(GLFW_INCLUDE_DIR ${GLFW_INCLUDE_DIR}/GLFW)


# Find libraries
set(debug_postfix .lib)
set(release_postfix .lib)
set(LIB_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/lib)

find_library(GLFW_LIBRARY-RELEASE
	NAMES
		glfw3${release_postfix}
		libglfw3${release_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)

find_library(GLFW_LIBRARY-DEBUG
	NAMES
		glfw3${debug_postfix}
		libglfw3${debug_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)
		
list(APPEND GLFW_LIBRARIES optimized ${GLFW_LIBRARY-RELEASE})
list(APPEND GLFW_RELEASE_LIBRARIES ${GLFW_LIBRARY-RELEASE})
mark_as_advanced(GLFW_LIBRARY-RELEASE)

list(APPEND GLFW_LIBRARIES debug ${GLFW_LIBRARY-DEBUG})
list(APPEND GLFW_DEBUG_LIBRARIES ${GLFW_LIBRARY-DEBUG})
mark_as_advanced(GLFW_LIBRARY-DEBUG)


# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG
	GLFW_INCLUDE_DIR)