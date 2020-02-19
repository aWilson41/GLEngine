# Find file for Eigen

# Find includes
set(header eigen3/Eigen/Cholesky)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(EIGEN_INCLUDE_DIR
  NAMES
	${header}
  PATHS
	${INCLUDE_SEARCH_DIR}
  NO_DEFAULT_PATH)
set(EIGEN_INCLUDE_DIR ${EIGEN_INCLUDE_DIR}/Eigen/eigen3)

# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Eigen DEFAULT_MSG
  EIGEN_INCLUDE_DIR)