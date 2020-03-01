cmake_minimum_required(VERSION 3.0.2)
 
project(GLEngine)

# Glob all the files together for use
file(GLOB CORE_HEADER_FILES Core/*.h)
file(GLOB CORE_SOURCE_FILES Core/*.cpp)
set(CORE_FILES ${CORE_HEADER_FILES} ${CORE_SOURCE_FILES})

file(GLOB RENDERING_HEADER_FILES Rendering/*.h)
file(GLOB RENDERING_SOURCE_FILES Rendering/*.cpp)
set(RENDERING_FILES ${RENDERING_HEADER_FILES} ${RENDERING_SOURCE_FILES})

file(GLOB DATASOURCE_HEADER_FILES Sources/*.h)
file(GLOB DATASOURCE_SOURCE_FILES Sources/*.cpp)
set(DATASOURCES_FILES ${DATASOURCE_HEADER_FILES} ${DATASOURCE_SOURCE_FILES})

file(GLOB IO_HEADER_FILES IO/*.h)
file(GLOB IO_SOURCE_FILES IO/*cpp)
set(IO_FILES ${IO_HEADER_FILES} ${IO_SOURCE_FILES})

file(GLOB_RECURSE SHADER_DIRECT_FILES Shaders/DirectRasterize/*glsl)
file(GLOB_RECURSE SHADER_DEFERRED_FILES Shaders/DeferredRasterize/*glsl)
file(GLOB_RECURSE SHADER_RAYCAST_FILES Shaders/Raycast/*glsl)
file(GLOB_RECURSE SHADER_RAYTRACE_FILES Shaders/Raytrace/*glsl)
file(GLOB_RECURSE SHADER_PATHTRACE_FILES Shaders/Pathtrace/*glsl)
set(SHADER_FILES ${SHADER_DIRECT_FILES} ${SHADER_DEFERRED_FILES} ${SHADER_RAYCAST_FILES} ${SHADER_RAYTRACE_FILES} ${SHADER_PATHTRACE_FILES})

file(GLOB LODEPNG_FILES LodePNG/lodepng.h LodePNG/lodepng.cpp)

set(HEADER_FILES ${CORE_HEADER_FILES} ${RENDERING_HEADER_FILES} ${DATASOURCE_HEADER_FILES} ${IO_HEADER_FILES} LodePNG/lodepng.h)

# Find all the packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
find_package(Eigen REQUIRED)
find_package(tbb REQUIRED)

#Set the includes and libraries
set(GLENGINE_INCLUDE_DIRS
	${GLM_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${EIGEN_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS}
	${TBB_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	Core
	IO
	LodePNG
	Rendering
	Sources)
set(GLENGINE_LIBRARIES
	${OPENGL_LIBRARIES}
	${GLFW_LIBRARIES}
	${GLEW_LIBRARIES}
	${TBB_LIBRARIES}
	${ASSIMP_LIBRARIES})

add_library(GLEngine STATIC
	${CORE_FILES}
	${RENDERING_FILES}
	${DATASOURCES_FILES}
	${IO_FILES}
	${SHADER_FILES}
	${LODEPNG_FILES})
target_include_directories(GLEngine PUBLIC ${GLENGINE_INCLUDE_DIRS})
target_link_libraries(GLEngine ${GLENGINE_LIBRARIES})

# Enable parallel build for msvc
if(WIN32 AND MSVC)
	add_definitions(/MP)
endif()
			
# Sort these into folders/filters
source_group("Core" FILES ${CORE_FILES})
source_group("Rendering" FILES ${RENDERING_FILES})
source_group("Sources" FILES ${DATASOURCES_FILES})
source_group("IO" FILES ${IO_FILES})
source_group("Resources" FILES ${IMAGERSC})
source_group("Shaders\\Direct" FILES ${SHADER_DIRECT_FILES})
source_group("Shaders\\Deferred" FILES ${SHADER_DEFERRED_FILES})
source_group("Shaders\\Raycast" FILES ${SHADER_RAYCAST_FILES})
source_group("Shaders\\Raytrace" FILES ${SHADER_RAYTRACE_FILES})
source_group("Shaders\\Pathtrace" FILES ${SHADER_PATHTRACE_FILES})
source_group("LodePNG" FILES ${LODEPNG_FILES})

install(TARGETS GLEngine EXPORT GLEngine_TARGETS DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/GLEngine)

# Add a build command to copy the shaders to the build directory post build
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