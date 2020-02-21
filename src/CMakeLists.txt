cmake_minimum_required(VERSION 3.0.2)
 
project(GLEngine)

# Glob all the files together for use
file(GLOB ENGINE_HEADER_FILES *.h)
file(GLOB ENGINE_SOURCE_FILES *.cpp)
set(ENGINE_FILES ${ENGINE_HEADER_FILES} ${ENGINE_SOURCE_FILES})

file(GLOB ENGINE_RENDERING_HEADER_FILES Rendering/*.h)
file(GLOB ENGINE_RENDERING_SOURCE_FILES Rendering/*.cpp)
set(ENGINE_RENDERING_FILES ${ENGINE_RENDERING_HEADER_FILES} ${ENGINE_RENDERING_SOURCE_FILES})

file(GLOB ENGINE_DATASOURCE_HEADER_FILES Sources/*.h)
file(GLOB ENGINE_DATASOURCE_SOURCE_FILES Sources/*.cpp)
set(ENGINE_DATASOURCES_FILES ${ENGINE_DATASOURCE_HEADER_FILES} ${ENGINE_DATASOURCE_SOURCE_FILES})

file(GLOB ENGINE_IO_HEADER_FILES IO/*.h)
file(GLOB ENGINE_IO_SOURCE_FILES IO/*cpp)
set(ENGINE_IO_FILES ${ENGINE_IO_HEADER_FILES} ${ENGINE_IO_SOURCE_FILES})

file(GLOB_RECURSE SHADER_DIRECT_FILES Shaders/DirectRasterize/*glsl)
file(GLOB_RECURSE SHADER_DEFERRED_FILES Shaders/DeferredRasterize/*glsl)
file(GLOB_RECURSE SHADER_RAYCAST_FILES Shaders/Raycast/*glsl)
file(GLOB_RECURSE SHADER_RAYTRACE_FILES Shaders/Raytrace/*glsl)
file(GLOB_RECURSE SHADER_PATHTRACE_FILES Shaders/Pathtrace/*glsl)
set(SHADER_FILES ${SHADER_DIRECT_FILES} ${SHADER_DEFERRED_FILES} ${SHADER_RAYCAST_FILES} ${SHADER_RAYTRACE_FILES} ${SHADER_PATHTRACE_FILES})

file(GLOB LODEPNG_FILES LodePNG/lodepng.h LodePNG/lodepng.cpp)

set(ENGINE_INCLUDES ${ENGINE_HEADER_FILES} ${ENGINE_RENDERING_HEADER_FILES} ${ENGINE_DATASOURCE_HEADER_FILES} ${ENGINE_IO_HEADER_FILES} LodePNG/lodepng.h)

# Find all the packages
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(Eigen REQUIRED)

#Set the includes and libraries
set(GLENGINE_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Rendering
	${CMAKE_CURRENT_SOURCE_DIR}/Sources
	${CMAKE_CURRENT_SOURCE_DIR}/IO
	${GLM_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${EIGEN_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIRS})

#set(GLENGINE_LIBRARIES
#	debug ${GLEW}/lib/Release/x64/glew32.lib optimized ${GLEW}/lib/Release/x64/glew32.lib
#	debug ${GLFW}/lib/glfw3d.lib optimized ${GLFW}/lib/glfw3.lib
#	debug ${OPENGL_LIBRARIES} optimized ${OPENGL_LIBRARIES})

#if (USE_EIGEN)
#	set(GLENGINE_LIBRARIES ${GLENGINE_LIBRARIES} Eigen3::Eigen)
#	add_definitions(-DUSEEIGEN)
#endif()

add_library(GLEngine STATIC
	${ENGINE_FILES}
	${ENGINE_RENDERING_FILES}
	${ENGINE_DATASOURCES_FILES}
	${ENGINE_IO_FILES}
	${SHADER_FILES}
	${LODEPNG_FILES})
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
			${CMAKE_CURRENT_SOURCE_DIR}/Shaders
			${CMAKE_BINARY_DIR}/Shaders)
add_custom_command(
	TARGET GLEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/Shaders
			${CMAKE_BINARY_DIR}/Release/Shaders)
add_custom_command(
	TARGET GLEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/Shaders
			${CMAKE_BINARY_DIR}/Debug/Shaders)
			
# Sort these into folders/filters
source_group("Main" FILES ${ENGINE_FILES})
source_group("Rendering" FILES ${ENGINE_RENDERING_FILES})
source_group("Sources" FILES ${ENGINE_DATASOURCES_FILES})
source_group("IO" FILES ${ENGINE_IO_FILES})
source_group("Resources" FILES ${IMAGERSC})
source_group("Shaders\\Direct" FILES ${SHADER_DIRECT_FILES})
source_group("Shaders\\Deferred" FILES ${SHADER_DEFERRED_FILES})
source_group("Shaders\\Raycast" FILES ${SHADER_RAYCAST_FILES})
source_group("Shaders\\Raytrace" FILES ${SHADER_RAYTRACE_FILES})
source_group("Shaders\\Pathtrace" FILES ${SHADER_PATHTRACE_FILES})
source_group("LodePNG" FILES ${LODEPNG_FILES})

install(TARGETS GLEngine DESTINATION lib)
install(FILES ${ENGINE_INCLUDES} DESTINATION include/GLEngine)
install(FILES ${SHADER_FILES} DESTINATION shaders/GLEngine)