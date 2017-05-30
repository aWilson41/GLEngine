#include "App.h"
#include "ResourceLoader.h"

#include <iostream>

void PrintShaderLinkingError(GLuint shaderProgram);

void App::Initialize()
{
   
}

void App::UnLoad()
{
    scene.cleanup();
}

void App::Load()
{
    // Generate geometry
    ResourceLoader::LoadAllMeshesFromFolder("Content/*", &scene);

    // Load shaders
    glBindAttribLocation(shaderProgram, 0, "inVertex");
    glBindAttribLocation(shaderProgram, 1, "inNormal");

    shaders.push_back(ResourceLoader::LoadShaderFromPath("Content/Shaders/VertexShader.glsl", GL_VERTEX_SHADER));   
    glAttachShader(shaderProgram, shaders[0]);

    shaders.push_back(ResourceLoader::LoadShaderFromPath("Content/Shaders/PhongFragmentShader.glsl", GL_FRAGMENT_SHADER));
    glAttachShader(shaderProgram, shaders[1]);

    // Where the shaders are inspected, optimized, and uploaded to the GPU
    glLinkProgram(shaderProgram);
    int isLinked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == false)
        PrintShaderLinkingError(shaderProgram);

    glUseProgram(shaderProgram);

    // Set the projection
    int width = 1;
    int height = 1;
    SDL_GetWindowSize(mWindow, &width, &height);
    scene.cam.SetPerspective(70.0f, (float)width / height, 0.01f, 2000.0f);

    // Set the view
    scene.cam.SetCamPos(glm::vec3(0.0f, 20.0f, -40.0f));
    scene.cam.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    // Set the scene directional light
	scene.dirLight = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

    // Get the uniforms
    for (UINT i = 0; i < scene.mesh.size(); i++)
    {
        scene.mesh[i].mvpID = glGetUniformLocation(shaderProgram, "mvp");
        scene.mesh[i].tInvModelID = glGetUniformLocation(shaderProgram, "tInvModel");
        scene.mesh[i].mat->matID = glGetUniformLocation(shaderProgram, "mat");
    }
    
    scene.cam.viewDirID = glGetUniformLocation(shaderProgram, "viewDir");
    scene.dirLightID = glGetUniformLocation(shaderProgram, "dirLight");
    glUniform3fv(scene.dirLightID, 1, &scene.dirLight[0]);
}

void App::Update()
{
	if (IsCurrentKeyDown(27) && !IsPreviousKeyDown(27))
		Quit();

    // Model controls
	// Rotation
	if (IsCurrentKeyDown('d'))
		scene.mesh[0].model *= MathHelper::MatrixRotateZ(0.05f);
	if (IsCurrentKeyDown('a'))
		scene.mesh[0].model *= MathHelper::MatrixRotateZ(-0.05f);

	if (IsCurrentKeyDown('w'))
		scene.mesh[0].model *= MathHelper::MatrixRotateX(-0.05f);
	if (IsCurrentKeyDown('s'))
		scene.mesh[0].model *= MathHelper::MatrixRotateX(0.05f);

	if (IsCurrentKeyDown('e'))
		scene.mesh[0].model *= MathHelper::MatrixRotateY(0.05f);
	if (IsCurrentKeyDown('q'))
		scene.mesh[0].model *= MathHelper::MatrixRotateY(-0.05f);

    // Translation
	/*if(IsCurrentKeyDown('g'))
		scene.mesh[0].model *= MathHelper::MatrixTranslate(-0.1f, 0.0f, 0.0f);
	if (IsCurrentKeyDown('h'))
		scene.mesh[0].model *= MathHelper::MatrixTranslate(0.1f, 0.0f, 0.0f);

    if (IsCurrentKeyDown('z'))
        scene.mesh[0].model *= MathHelper::MatrixTranslate(0.0f, 0.0f, -0.1f);
    if (IsCurrentKeyDown('x'))
        scene.mesh[0].model *= MathHelper::MatrixTranslate(0.0f, 0.0f, 0.1f);*/

	// Scaling
	if (IsCurrentKeyDown('p'))
		scene.mesh[0].model *= MathHelper::MatrixScale(1.02f);
	if (IsCurrentKeyDown('o'))
		scene.mesh[0].model *= MathHelper::MatrixScale(0.98f);


    // Camera controls
    if (IsCurrentKeyDown('j'))
        scene.cam.SetCameraView(MathHelper::MatrixRotateY(0.02f));

    if (IsCurrentKeyDown('l'))
       scene.cam.SetCameraView(MathHelper::MatrixRotateY(-0.02f));

    if (IsCurrentKeyDown('i'))
       scene.cam.SetCameraView(MathHelper::MatrixRotateX(-0.02f));

    if (IsCurrentKeyDown('k'))
       scene.cam.SetCameraView(MathHelper::MatrixRotateX(0.02f));

    if (IsCurrentKeyDown('g'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, 0.2f));

    if (IsCurrentKeyDown('t'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, -0.2f));

    if (IsCurrentKeyDown('h'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(-0.2f, 0.0f, 0.0f));

    if (IsCurrentKeyDown('f'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.2f, 0.0f, 0.0f));

	/*if (IsCurrentKeyDown('m') && !IsPreviousKeyDown('m'))
	Utility::SaveScreenshot(sDevice.screen.GetBuffer(), sDevice.screen.GetScreenWidth(), sDevice.screen.GetScreenHeight(), "Output/output.png");*/
}

void App::Draw()
{
    scene.draw();
}

void PrintShaderLinkingError(GLuint shaderProgram)
{
    printf("=======================================\n");
    printf("Shader linking failed : \n");

    // Find length of shader info log
    int maxLength;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

    printf("Info Length : %i\n", maxLength);

    // Get shader info log
    char* shaderProgramInfoLog = new char[maxLength];
    glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

    printf("Linker error message : %s\n", shaderProgramInfoLog);

    /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
    /* In this simple program, we'll just leave */
    delete shaderProgramInfoLog;
    return;
}