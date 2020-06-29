# GLEngine
An OpenGL abstraction I like to use in my other projects. May update to Vulkan soon. Use the CMakeLists.txt to build the static library.

The project is a superbuild so there is no need to go find the dependencies, just build with cmake. Only works for Windows right now. Config file in src/ directory.

The rendering archicture is much like VTK but simplified and more directly coupled with OpenGL. One major difference is that it pairs an entity-component architecture with pipeline rendering architecture. This keeps it highly extensible for both data processing and dynamic complex scene's.

# Modules

Common: Various common function throughout all other modules

IO: Provides various io filters

Filtering: Provides various data processing filters

Scene: Provides the Scene, SceneObjects, & Components

Rendering: Provides the renderers, render passes, & other useful rendering classes

Sources: Provides data sources

# Scene

The Scene contains a set of SceneObjects not yet in graph representation.

SceneObjects contain a set of components with each component adding some functionality.

Components implement functionality and should be designed with Signals/Slots to ensure decoupled implementations.

# Rendering

The renderer will render a scene to a render target. This can then be provided to a RenderWindow (common module). Multiple renderers exist.
 
 * Renderer: Does direct rendering

 * DeferredRenderer: Allows one to specify sets of render passes. Provided within the Rendering module are numerous passes.

 * RayTraceRenderer: A compute shader ray tracer.

 * RayTrace2dRenderer: A very unique compute shader renderer for 2d rendering.

# Filtering, IO, Sources

Contains most of the filtering. Currently just image and poly data filters, IO, & sources exist.

# Dependencies

OpenGL for rendering

GLEW to talk to OpenGL

GLFW for window and input management

GLM for vector math

Assimp for scene loading

tbb for parallelism

Eigen for matrix math (sparse martices, linear solvers, etc)