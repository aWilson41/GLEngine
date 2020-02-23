# Find file for GLM

# Find includes
set(header glm/glm.hpp)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(GLM_INCLUDE_DIR
  NAMES
	${header}
  PATHS
	${INCLUDE_SEARCH_DIR}
  NO_DEFAULT_PATH)
  
message("GLM Include is: ${GLM_INCLUDE_DIR}")

# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG
	GLM_INCLUDE_DIR)