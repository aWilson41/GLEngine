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
    glDeleteVertexArrays(1, &vaoID);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal) * scene.mesh[0].vertices.size(), &scene.mesh[0].vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), BUFFER_OFFSET(sizeof(float) * 3));

    // Setup the index buffer
    glGenBuffers(1, &scene.mesh[0].indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.mesh[0].indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene.mesh[0].indices.size() * sizeof(GLuint), &scene.mesh[0].indices[0], GL_STATIC_DRAW);


    // Load shaders
    glBindAttribLocation(shaderProgram, 0, "inVertex");
    glBindAttribLocation(shaderProgram, 1, "inNormal");

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

    // Set the projection
    int width = 1;
    int height = 1;
    SDL_GetWindowSize(mWindow, &width, &height);
    scene.cam.SetPerspective(70.0f, (float)width / height, 0.01f, 2000.0f);

    // Set the view
    scene.cam.SetCamPos(glm::vec3(0.0f, 0.0f, -20.0f));
    scene.cam.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    // Set the scene directional light
	scene.dirLight = glm::vec3(0.0f, 0.0f, 1.0f);

    // Pass the uniforms
    scene.mesh[0].modelMatID = glGetUniformLocation(shaderProgram, "modelMat");
    scene.cam.viewMatID = glGetUniformLocation(shaderProgram, "viewMat");
    scene.cam.projMatID = glGetUniformLocation(shaderProgram, "projMat");
    scene.mesh[0].tInvModelViewMatID = glGetUniformLocation(shaderProgram, "tInvModelViewMat");

    scene.mesh[0].mat->matID = glGetUniformLocation(shaderProgram, "mat");
    glUniformMatrix4fv(scene.mesh[0].mat->matID, 1, GL_FALSE, &scene.mesh[0].mat->mat[0][0]);

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
	if(IsCurrentKeyDown('g'))
		scene.mesh[0].model *= MathHelper::MatrixTranslate(-0.1f, 0.0f, 0.0f);
	if (IsCurrentKeyDown('h'))
		scene.mesh[0].model *= MathHelper::MatrixTranslate(0.1f, 0.0f, 0.0f);

    if (IsCurrentKeyDown('z'))
        scene.mesh[0].model *= MathHelper::MatrixTranslate(0.0f, 0.0f, -0.1f);
    if (IsCurrentKeyDown('x'))
        scene.mesh[0].model *= MathHelper::MatrixTranslate(0.0f, 0.0f, 0.1f);

	// Scaling
	if (IsCurrentKeyDown('p'))
		scene.mesh[0].model *= MathHelper::MatrixScale(1.02f);
	if (IsCurrentKeyDown('o'))
		scene.mesh[0].model *= MathHelper::MatrixScale(0.98f);

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
    // We pass the model, view, proj, and tInvModelViewMat every time. (we should only do this when they change though)
    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(scene.cam.GetView()) * glm::mat3(scene.mesh[0].model)));
    glUniformMatrix4fv(scene.mesh[0].modelMatID, 1, GL_FALSE, &scene.mesh[0].model[0][0]);
    glUniformMatrix4fv(scene.cam.viewMatID, 1, GL_FALSE, &scene.cam.GetView()[0][0]);
    glUniformMatrix4fv(scene.cam.projMatID, 1, GL_FALSE, &scene.cam.GetProj()[0][0]);
    glUniformMatrix3fv(scene.mesh[0].tInvModelViewMatID, 1, GL_FALSE, &normalMat[0][0]);

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