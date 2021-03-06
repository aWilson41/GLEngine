#include "LightingPass.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Shaders.h"

LightingPass::LightingPass() :
	FramePass("Lighting Pass")
{
	shader = Shaders::loadVSFSShader("Lighting_Pass",
		"Shaders/DeferredRasterize/Passes/quadVS.glsl",
		"Shaders/DeferredRasterize/Passes/lightPassFS.glsl");
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);
	glUniform1i(glGetUniformLocation(shaderID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(shaderID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(shaderID, "gDiffuseColor"), 2);
	glUniform1i(glGetUniformLocation(shaderID, "gAmbientColor"), 3);
	glUseProgram(0);

	setNumberOfInputPorts(4);
	setNumberOfOutputPorts(1);
}

void LightingPass::bindInputs()
{
	if (cam == nullptr || scene == nullptr)
	{
		printf("Scene or Camera missing for LightingPass\n");
		return;
	}

	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);

	// Set the scene uniforms
	GLuint lightDirLocation = glGetUniformLocation(shaderID, "lightDir");
	if (lightDirLocation != -1)
		glUniform3fv(lightDirLocation, 1, &scene->getLightDir()[0]);
	GLuint viewDirLocation = glGetUniformLocation(shaderID, "viewDir");
	if (viewDirLocation != -1)
		glUniform3fv(viewDirLocation, 1, &cam->getLookDir()[0]);
	GLuint invViewProjLocation = glGetUniformLocation(shaderID, "invViewProj");
	if (invViewProjLocation != -1)
	{
		glm::mat4 invViewProj = glm::inverse(cam->proj * cam->view);
		glUniformMatrix4fv(invViewProjLocation, 1, GL_FALSE, &invViewProj[0][0]);
	}
	GLuint eyePosLocation = glGetUniformLocation(shaderID, "eyePos");
	if (eyePosLocation != -1)
		glUniform3fv(eyePosLocation, 1, &cam->getViewPos()[0]);

	for (UINT i = 0; i < inputs.size(); i++)
	{
		inputs[i]->bind(i);
	}
}


void LightingPass::resize(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		if (!framebuffer->generate(width, height, {
			{ Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr } }))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);
}