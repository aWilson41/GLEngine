cmake_minimum_required(VERSION 3.0.2)
 
PROJECT(GLEngine)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeFiles")
#message(WARNING "${CMAKE_SOURCE_DIR}/CMakeFiles/")

# Glob all the files together for use
file(GLOB ENGINE_FILES src/*.h src/*.cpp)
file(GLOB ENGINE_RENDERING_FILES src/Rendering/*.h src/Rendering/*.cpp)

file(GLOB_RECURSE SHADER_DIRECT_FILES src/Shaders/DirectRasterize/*glsl)
file(GLOB_RECURSE SHADER_DEFERRED_FILES src/Shaders/DeferredRasterize/*glsl)
file(GLOB_RECURSE SHADER_RAYCAST_FILES src/Shaders/Raycast/*glsl)
file(GLOB_RECURSE SHADER_RAYTRACE_FILES src/Shaders/Raytrace/*glsl)
file(GLOB_RECURSE SHADER_PATHTRACE_FILES src/Shaders/Pathtrace/*glsl)
set(SHADER_FILES ${SHADER_DIRECT_FILES} ${SHADER_DEFERRED_FILES} ${SHADER_RAYCAST_FILES} ${SHADER_RAYTRACE_FILES} ${SHADER_PATHTRACE_FILES})

file(GLOB LODEPNG_FILES src/LodePNG/lodepng.h src/LodePNG/lodepng.cpp)

# Find all the packages and stuff
find_package(OpenGL REQUIRED)
find_path(GLM 
	NAMES "/glm" 
	PATHS ENV PATH)
find_path(GLEW 
	NAMES "/include/GL/glew.h"
	PATHS ENV PATH)
find_path(GLFW
	NAMES "/include/GLFW"
	PATHS ENV PATH)
# Find quiet/optional packages (don't complain when they aren't found)
find_package(Eigen3 3.3 QUIET)

if(USE_EIGEN OR (TARGET Eigen3::Eigen))
	option(USE_EIGEN "Toggle for Eigen usage." ON)
else()
	option(USE_EIGEN "Toggle for Eigen usage." OFF)
endif()

#Set the includes and libraries
set(GLENGINE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/Rendering ${CMAKE_CURRENT_BINARY_DIR} ${GLM}/glm ${GLEW}/include ${GLFW}/include ${OPENGL_INCLUDE_DIRS})

set(GLENGINE_LIBRARIES
	debug ${GLEW}/lib/Release/x64/glew32.lib optimized ${GLEW}/lib/Release/x64/glew32.lib
	debug ${GLFW}/lib/glfw3d.lib optimized ${GLFW}/lib/glfw3.lib
	debug ${OPENGL_LIBRARIES} optimized ${OPENGL_LIBRARIES})

if (USE_EIGEN)
	set(GLENGINE_LIBRARIES ${GLENGINE_LIBRARIES} Eigen3::Eigen)
	add_definitions(-DUSEEIGEN)
endif()

add_library(GLEngine STATIC ${ENGINE_FILES} ${ENGINE_RENDERING_FILES} ${SHADER_FILES} ${LODEPNG_FILES})
target_include_directories(GLEngine PUBLIC ${GLENGINE_INCLUDE_DIRS})
target_link_libraries(GLEngine ${GLENGINE_LIBRARIES})

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
	# Enable parallel build for msvc
	add_definitions(/MP)
  endif()
endif()

# Add a build command to copy the shaders to the build directory post build
add_custom_command(
	TARGET GLEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders
			${CMAKE_BINARY_DIR}/Shaders)
add_custom_command(
	TARGET GLEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders
			${CMAKE_BINARY_DIR}/Release/Shaders)
add_custom_command(
	TARGET GLEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders
			${CMAKE_BINARY_DIR}/Debug/Shaders)
			
# Sort these into folders/filters
source_group("Main" FILES ${ENGINE_FILES})
source_group("Rendering" FILES ${ENGINE_RENDERING_FILES})
source_group("Resources" FILES ${IMAGERSC})
source_group("Shaders\\Direct" FILES ${SHADER_DIRECT_FILES})
source_group("Shaders\\Deferred" FILES ${SHADER_DEFERRED_FILES})
source_group("Shaders\\Raycast" FILES ${SHADER_RAYCAST_FILES})
source_group("Shaders\\Raytrace" FILES ${SHADER_RAYTRACE_FILES})
source_group("Shaders\\Pathtrace" FILES ${SHADER_PATHTRACE_FILES})
source_group("LodePNG" FILES ${LODEPNG_FILES})