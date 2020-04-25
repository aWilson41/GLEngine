# Find file for GLEW
set(libname "GLEW")


# Find includes
set(header GL/glew.h)
set(INCLUDE_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/include)

find_path(${libname}_INCLUDE_DIR
	NAMES
		${header}
	PATHS
		${INCLUDE_SEARCH_DIR}
	NO_DEFAULT_PATH)


# Find libraries
set(debug_postfix d.lib)
set(release_postfix .lib)
set(LIB_SEARCH_DIR ${CMAKE_INSTALL_PREFIX}/lib)

find_library(${libname}_LIBRARY_RELEASE
	NAMES
		glew${release_postfix}
		libglew${release_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Release
	NO_DEFAULT_PATH)

find_library(${libname}_LIBRARY_DEBUG
	NAMES
		glew${debug_postfix}
		libglew${debug_postfix}
	PATHS
		${LIB_SEARCH_DIR}
		${LIB_SEARCH_DIR}/Debug
	NO_DEFAULT_PATH)


# Check found
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${libname} DEFAULT_MSG
	${libname}_INCLUDE_DIR)


# Add the library as imported
if(${libname}_FOUND)
	if (NOT TARGET ${libname})
		add_library(${libname} UNKNOWN IMPORTED)
	endif()
	if (${libname}_LIBRARY_RELEASE)
		set_property(TARGET ${libname} APPEND PROPERTY
			IMPORTED_CONFIGURATIONS RELEASE)
		set_target_properties(${libname} PROPERTIES
			IMPORTED_LOCATION_RELEASE "${${libname}_LIBRARY_RELEASE}")
	endif()
	if (${libname}_LIBRARY_DEBUG)
		set_property(TARGET ${libname} APPEND PROPERTY
			IMPORTED_CONFIGURATIONS DEBUG)
		set_target_properties(${libname} PROPERTIES
			IMPORTED_LOCATION_DEBUG "${${libname}_LIBRARY_DEBUG}")
	endif()
	set_target_properties(${libname} PROPERTIES
#		INTERFACE_COMPILE_OPTIONS "${PC_${libname}_CFLAGS_OTHER}"
		INTERFACE_INCLUDE_DIRECTORIES "${${libname}_INCLUDE_DIR}")
endif()

mark_as_advanced(${libname}_INCLUDE_DIR)
mark_as_advanced(${libname}_LIBRARY_RELEASE)
mark_as_advanced(${libname}_LIBRARY_DEBUG)