# GLEngine
Just a little OpenGL abstraction I like to use in my other projects. Architecture inspired by VTK. Use the CMakeLists.txt to build the static library.
Currently utilizes OpenGL, GLEW, GLM, Eigen, and Assimp. Build only works on Windows, may be reworked for other platforms some other time. Build is a superbuild so no need to go find all the dependencies.

Cmake config file provided in src/ directory. To use with your cmakes script find_package set GLEngine_DIR to there. Defines GLEngine_INCLUDE_DIRS and GLEngine_LIBRARIES.

OpenGL for rendering

GLEW to talk to OpenGL

GLFW for window and input management

GLM for convienent math

Assimp for scene loading

tbb for parallelism

Eigen for various maths (sparse martices, linear solvers, etc)