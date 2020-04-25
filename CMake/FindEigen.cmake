# Find file for Eigen
set(libname "Eigen")


# Find includes
set(header eigen3/Eigen/Cholesky)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(${libname}_INCLUDE_DIR
	NAMES
		${header}
	PATHS
		${INCLUDE_SEARCH_DIR}
	NO_DEFAULT_PATH)
set(${libname}_INCLUDE_DIR "${${libname}_INCLUDE_DIR}/eigen3")


# Check found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${libname} DEFAULT_MSG
	${libname}_INCLUDE_DIR)


# Add the library as imported
if(${libname}_FOUND)
	if (NOT TARGET ${libname})
		add_library(${libname} INTERFACE IMPORTED)
	endif()
	target_include_directories(${libname} INTERFACE "${${libname}_INCLUDE_DIR}")
endif()

mark_as_advanced(${libname}_INCLUDE_DIR)