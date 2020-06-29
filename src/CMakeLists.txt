cmake_minimum_required(VERSION 3.9)
 
project(GLEngine)

# Find all the external dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
find_package(assimp REQUIRED)
find_package(Eigen REQUIRED)
find_package(tbb REQUIRED)


# Glob all the files together for use
file(GLOB COMMON_HEADER_FILES Common/*.h)
file(GLOB COMMON_SOURCE_FILES Common/*.cpp)
set(COMMON_FILES ${COMMON_HEADER_FILES} ${COMMON_SOURCE_FILES})

file(GLOB SCENE_HEADER_FILES Scene/*.h)
file(GLOB SCENE_SOURCE_FILES Scene/*.cpp)
set(SCENE_FILES ${SCENE_HEADER_FILES} ${SCENE_SOURCE_FILES})

file(GLOB RENDERING_HEADER_FILES Rendering/*.h)
file(GLOB RENDERING_SOURCE_FILES Rendering/*.cpp)
set(RENDERING_FILES ${RENDERING_HEADER_FILES} ${RENDERING_SOURCE_FILES})

file(GLOB DATASOURCE_HEADER_FILES DataSources/*.h)
file(GLOB DATASOURCE_SOURCE_FILES DataSources/*.cpp)
set(DATASOURCES_FILES ${DATASOURCE_HEADER_FILES} ${DATASOURCE_SOURCE_FILES})

file(GLOB LODEPNG_HEADER_FILES IO/LodePNG/lodepng.h)
file(GLOB LODEPNG_SOURCE_FILES IO/LodePNG/lodepng.cpp)
set(LODEPNG_FILES ${LODEPNG_HEADER_FILES} ${LODEPNG_SOURCE_FILES})

file(GLOB IO_HEADER_FILES IO/*.h)
file(GLOB IO_SOURCE_FILES IO/*cpp)
set(IO_FILES ${IO_HEADER_FILES} ${IO_SOURCE_FILES})

file(GLOB_RECURSE SHADER_DIRECT_FILES Shaders/DirectRasterize/*glsl)
file(GLOB_RECURSE SHADER_DEFERRED_FILES Shaders/DeferredRasterize/*glsl)
file(GLOB_RECURSE SHADER_RAYCAST_FILES Shaders/Raycast/*glsl)
file(GLOB_RECURSE SHADER_RAYTRACE_FILES Shaders/Raytrace/*glsl)
file(GLOB_RECURSE SHADER_PATHTRACE_FILES Shaders/Pathtrace/*glsl)
file(GLOB_RECURSE SHADER_COMMON_FILES Shaders/Common/*.glsl)
set(SHADER_FILES ${SHADER_DIRECT_FILES} ${SHADER_DEFERRED_FILES} ${SHADER_RAYCAST_FILES} ${SHADER_RAYTRACE_FILES} ${SHADER_PATHTRACE_FILES} ${SHADER_COMMON_FILES})

set(HEADER_FILES ${COMMON_HEADER_FILES} ${SCENE_HEADER_FILES} ${RENDERING_HEADER_FILES} ${DATASOURCE_HEADER_FILES} ${IO_HEADER_FILES} ${LODEPNG_HEADER_FILES})

# Create all the libs and add_dependency

# Create the library
add_library(GLEngine STATIC
	${COMMON_FILES}
	${SCENE_FILES}
	${RENDERING_FILES}
	${DATASOURCES_FILES}
	${IO_FILES}
	${SHADER_FILES}
	${LODEPNG_FILES})
# Link the dependencies
target_link_libraries(GLEngine
	PUBLIC
		OpenGL::GL
		GLM GLEW GLFW
		Eigen
		assimp
		tbb)
# Include the subdirectories
target_include_directories(GLEngine
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		Common
		Scene
		IO
		IO/LodePNG
		Rendering
		DataSources)
set_target_properties(GLEngine PROPERTIES DEBUG_POSTFIX "d")


# Enable parallel build for msvc
if(WIN32 AND MSVC)
	add_definitions(/MP)
endif()
			
# Sort these into folders/filters
source_group("Common" FILES ${COMMON_FILES})
source_group("Scene" FILES ${SCENE_FILES})
source_group("Rendering" FILES ${RENDERING_FILES})
source_group("DataSources" FILES ${DATASOURCES_FILES})
source_group("IO" FILES ${IO_FILES})
source_group("IO\\LodePNG" FILES ${LODEPNG_FILES})
source_group("Resources" FILES ${IMAGERSC})
source_group("Shaders\\Direct" FILES ${SHADER_DIRECT_FILES})
source_group("Shaders\\Deferred" FILES ${SHADER_DEFERRED_FILES})
source_group("Shaders\\Raycast" FILES ${SHADER_RAYCAST_FILES})
source_group("Shaders\\Raytrace" FILES ${SHADER_RAYTRACE_FILES})
source_group("Shaders\\Pathtrace" FILES ${SHADER_PATHTRACE_FILES})
source_group("Shaders\\Common" FILES ${SHADER_COMMON_FILES})

install(TARGETS GLEngine EXPORT GLEngine_TARGETS DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/GLEngine)

# Add a build command to copy the shaders to the build directory post build
# todo: Switch to install?
add_custom_command(
	TARGET GLEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/Shaders
			${CMAKE_INSTALL_PREFIX}/bin/shaders)

# Export the target file for the config file to use later			
export(EXPORT GLEngine_TARGETS
	FILE ${CMAKE_CURRENT_BINARY_DIR}/GLEngineTargets.cmake)

configure_file(GLEngineConfig.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/GLEngineConfig.cmake"
	@ONLY)