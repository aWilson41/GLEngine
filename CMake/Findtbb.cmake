# Find file for tbb

# Find includes
set(header tbb/tbb.h)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(TBB_INCLUDE_DIR
  NAMES
	${header}
  PATHS
	${INCLUDE_SEARCH_DIR}
  NO_DEFAULT_PATH)


# Find libraries
set(LIB_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/lib)
find_library(TBB_LIBRARY-RELEASE
	NAMES
		tbb.lib
		libtbb.lib
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)
find_library(TBB_LIBRARY-DEBUG
	NAMES
		tbb_debug.lib
		libtbb_debug.lib
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Debug
	NO_DEFAULT_PATH)

if (EXISTS ${TBB_LIBRARY-RELEASE})
	list(APPEND TBB_LIBRARIES optimized ${TBB_LIBRARY-RELEASE})
	list(APPEND TBB_RELEASE_LIBRARIES ${TBB_LIBRARY-RELEASE})
endif()
mark_as_advanced(TBB_LIBRARY-RELEASE)

if (EXISTS ${TBB_LIBRARY-DEBUG})
	list(APPEND TBB_LIBRARIES debug ${TBB_LIBRARY-DEBUG})
	list(APPEND TBB_DEBUG_LIBRARIES ${TBB_LIBRARY-DEBUG})
endif()
mark_as_advanced(TBB_LIBRARY-DEBUG)


# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tbb DEFAULT_MSG
  TBB_INCLUDE_DIR)