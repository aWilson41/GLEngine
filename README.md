# GLEngine
Just a little OpenGL abstraction I like to use in my other projects. Architecture inspired by VTK. Use the CMakeLists.txt to build the static library.
Currently utilizes OpenGL, GLEW, GLM, Eigen, and Assimp. Build only works on Windows, may be reworked for other platforms some other time. Build is a superbuild so no need to go find all the dependencies.

OpenGL for rendering

GLEW to talk to OpenGL

GLM for convienent math

Assimp for scene loading

Eigen for various maths (sparse martices, linear solvers, etc)