#include "App.h"
#include "ResourceLoader.h"

#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

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
    scene.cam.SetCamPos(glm::vec3(0.0f, 0.0f, -20.0f));
    scene.cam.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    // Set the scene directional light
	scene.dirLight = glm::vec3(0.0f, 0.0f, -1.0f);

    // Pass the uniforms
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
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, 0.1f));

    if (IsCurrentKeyDown('t'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, -0.1f));

    if (IsCurrentKeyDown('h'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(-0.1f, 0.0f, 0.0f));

    if (IsCurrentKeyDown('f'))
       scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.1f, 0.0f, 0.0f));

	/*if (IsCurrentKeyDown('m') && !IsPreviousKeyDown('m'))
	Utility::SaveScreenshot(sDevice.screen.GetBuffer(), sDevice.screen.GetScreenWidth(), sDevice.screen.GetScreenHeight(), "Output/output.png");*/
}

void App::Draw()
{
    glm::mat4 view = scene.cam.GetView();
    glm::vec3 viewDir = -glm::vec3(view[0][2], view[1][2], view[2][2]);
    glm::mat4 proj = scene.cam.GetProj();

    glUniform3fv(scene.cam.viewDirID, 1, &viewDir[0]);
    glBindVertexArray(scene.vaoID);

    for (UINT i = 0; i < scene.mesh.size(); i++)
    {
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(glm::mat3(scene.mesh[i].model))));
        glm::mat4 mvp = proj * view * scene.mesh[i].model;

        glUniformMatrix4fv(scene.mesh[i].mat->matID, 1, GL_FALSE, &scene.mesh[i].mat->mat[0][0]);
        glUniformMatrix4fv(scene.mesh[i].mvpID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix3fv(scene.mesh[i].tInvModelID, 1, GL_FALSE, &normalMatrix[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER, scene.mesh[i].vboID);
        glDrawElements(GL_TRIANGLES, scene.mesh[i].indices.size(), GL_UNSIGNED_INT, 0);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
            std::cerr << error << std::endl;
    }
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