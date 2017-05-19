#include "App.h"
#include "ResourceLoader.h"
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
GLuint vaoID;

void PrintShaderLinkingError(GLuint shaderProgram);

void App::Initialize()
{
   
}

void App::UnLoad()
{
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &scene.mesh[0].vertexVboID);
}

void App::Load()
{
    // Generate geometry
    ResourceLoader::LoadAllMeshesFromFolder("Content/*", &scene);

    // Generate a vertex buffer
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glGenBuffers(1, &scene.mesh[0].vertexVboID);
    glBindBuffer(GL_ARRAY_BUFFER, scene.mesh[0].vertexVboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * scene.mesh[0].vertices.size(), &scene.mesh[0].vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexColor), BUFFER_OFFSET(12));

    // Setup the index buffer
    glGenBuffers(1, &scene.mesh[0].indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.mesh[0].indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene.mesh[0].indices.size() * sizeof(GLuint), &scene.mesh[0].indices[0], GL_STATIC_DRAW);


    // Load shaders
    glBindAttribLocation(shaderProgram, 0, "inVertex");
    glBindAttribLocation(shaderProgram, 1, "inColor");

    shaders.push_back(ResourceLoader::LoadShaderFromPath("Content/Shaders/VertexShader.glsl", GL_VERTEX_SHADER));   
    glAttachShader(shaderProgram, shaders[0]);

    shaders.push_back(ResourceLoader::LoadShaderFromPath("Content/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER));
    glAttachShader(shaderProgram, shaders[1]);

    // Where the shaders are inspected, optimized, and uploaded to the GPU
    glLinkProgram(shaderProgram);
    int isLinked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == false)
        PrintShaderLinkingError(shaderProgram);

    glUseProgram(shaderProgram);


    //// Bind back to nothing
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Set the view

    // Set the projection
    //scene.cam.SetPerspective(70.0f, (float)sDevice.screen.GetScreenWidth() / (float)sDevice.screen.GetScreenHeight(), 0.01f, 2000.0f);

    /*for (UINT i = 0; i < scene.mesh.size(); i++)
    {
        scene.mesh[i].world = MathHelper::MatrixTranslate(0.0f, -50.0f, 700.0f) * MathHelper::MatrixRotateY(0.0f) * MathHelper::MatrixScale(15.0f);
    }*/
    //scene.mesh[1].world = MathHelper::MatrixTranslate(0.0f, -150.0f, 700.0f) * MathHelper::MatrixRotateY(2.0f) * MathHelper::MatrixScale(15.0f);
    //scene.mesh[2].world = MathHelper::MatrixTranslate(0.0f, -150.0f, 700.0f) * MathHelper::MatrixRotateY(2.0f) * MathHelper::MatrixScale(15.0f);

    // Add a light if the scene doesn't have one
    /*if(scene.dirLight.size() == 0)
	    scene.dirLight.push_back(float4(0.0f, 0.0f, -1.0f, 0.0f));*/
}

void App::Update()
{
	if (IsCurrentKeyDown(27) && !IsPreviousKeyDown(27))
		Quit();

	// Rotation
	if (IsCurrentKeyDown('d'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateZ(0.05f);

	if (IsCurrentKeyDown('a'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateZ(-0.05f);

	if (IsCurrentKeyDown('w'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateX(0.05f);

	if (IsCurrentKeyDown('s'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateX(-0.05f);

	if (IsCurrentKeyDown('e'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateY(0.05f);

	if (IsCurrentKeyDown('q'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateY(-0.05f);

	if(IsCurrentKeyDown('g'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixTranslate(-0.2f, 0.0f, 0.0f);

	if (IsCurrentKeyDown('h'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixTranslate(0.2f, 0.0f, 0.0f);

	// Scaling
	if (IsCurrentKeyDown('p'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixScale(1.05f);

	if (IsCurrentKeyDown('o'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixScale(0.95f);

	// Camera controls
   //if (IsCurrentKeyDown('j'))
   //{
   //   //scene.mesh[0].mat->specularShine += 0.1f;
   //   //scene.dirLight = MathHelper::MatrixRotateY(0.05f) * scene.dirLight;
   //   scene.cam.SetCameraView(MathHelper::MatrixRotateY(0.02f));
   //}

   //if (IsCurrentKeyDown('l'))
   //{
   //   //scene.mesh[0].mat->specularShine -= 0.1f;
   //   //scene.dirLight = MathHelper::MatrixRotateY(-0.05f) * scene.dirLight;
   //   scene.cam.SetCameraView(MathHelper::MatrixRotateY(-0.02f));
   //}

   //if (IsCurrentKeyDown('i'))
   //{
   //   //scene.dirLight = MathHelper::MatrixRotateX(0.05f) * scene.dirLight;
   //   scene.cam.SetCameraView(MathHelper::MatrixRotateX(-0.02f));
   //}

   //if (IsCurrentKeyDown('k'))
   //{
   //   //scene.dirLight = MathHelper::MatrixRotateX(-0.05f) * scene.dirLight;
   //   scene.cam.SetCameraView(MathHelper::MatrixRotateX(0.02f));
   //}

   //if (IsCurrentKeyDown('g'))
   //   scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, 8.0f));

   //if (IsCurrentKeyDown('t'))
   //   scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, -8.0f));

   //if (IsCurrentKeyDown('h'))
   //   scene.cam.SetCameraView(MathHelper::MatrixTranslate(-8.0f, 0.0f, 0.0f));

   //if (IsCurrentKeyDown('f'))
   //   scene.cam.SetCameraView(MathHelper::MatrixTranslate(8.0f, 0.0f, 0.0f));

	/*if (IsCurrentKeyDown('m') && !IsPreviousKeyDown('m'))
		Utility::SaveScreenshot(sDevice.screen.GetBuffer(), sDevice.screen.GetScreenWidth(), sDevice.screen.GetScreenHeight(), "Output/output.png");*/
}

void App::Draw()
{
    glBindVertexArray(vaoID);
    glDrawElements(GL_TRIANGLES, scene.mesh[0].indices.size(), GL_UNSIGNED_INT, 0);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        std::cerr << error << std::endl;
}

void PrintShaderLinkingError(GLuint shaderProgram)
{
    std::cout << "=======================================\n";
    std::cout << "Shader linking failed : " << std::endl;

    // Find length of shader info log
    int maxLength;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

    std::cout << "Info Length : " << maxLength << std::endl;

    // Get shader info log
    char* shaderProgramInfoLog = new char[maxLength];
    glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

    std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

    /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
    /* In this simple program, we'll just leave */
    delete shaderProgramInfoLog;
    return;
}