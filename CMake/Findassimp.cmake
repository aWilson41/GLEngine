# Find file for assimp

# Find includes
set(header assimp/ai_assert.h)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(ASSIMP_INCLUDE_DIR
  NAMES
	${header}
  PATHS
	${INCLUDE_SEARCH_DIR}
  NO_DEFAULT_PATH)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_INCLUDE_DIR}/assimp)


# Find libraries
set(debug_postfix vc142-mtd.lib)
set(release_postfix vc142-mt.lib)
set(LIB_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/lib)

find_library(ASSIMP_LIBRARY-RELEASE
	NAMES
		assimp${release_postfix}
		libassimp${release_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)

find_library(ASSIMP_LIBRARY-DEBUG
	NAMES
		assimp${debug_postfix}
		libassimp${debug_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)
		
list(APPEND ASSIMP_LIBRARIES optimized ${ASSIMP_LIBRARY-RELEASE})
list(APPEND ASSIMP_RELEASE_LIBRARIES ${ASSIMP_LIBRARY-RELEASE})
mark_as_advanced(ASSIMP_LIBRARY-RELEASE)

list(APPEND ASSIMP_LIBRARIES debug ${ASSIMP_LIBRARY-DEBUG})
list(APPEND ASSIMP_DEBUG_LIBRARIES ${ASSIMP_LIBRARY-DEBUG})
mark_as_advanced(ASSIMP_LIBRARY-DEBUG)


# Complete
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(assimp DEFAULT_MSG
  ASSIMP_INCLUDE_DIR)