#include "DepthToRgbPass.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Shaders.h"

DepthToRgbPass::DepthToRgbPass() :
	FramePass("DepthToRgb_Pass")
{
	shader = Shaders::loadVSFSShader(passName,
		"Shaders/DeferredRasterize/Passes/quadVS.glsl",
		"Shaders/DeferredRasterize/Passes/depthToRgbPass.glsl");
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);
	glUniform1i(glGetUniformLocation(shaderID, "depthTex"), 0);
	glUseProgram(0);

	// Takes color and depth input and outputs color and depth
	setNumberOfInputPorts(1);
	setNumberOfOutputPorts(1);
}

void DepthToRgbPass::bindInputs()
{
	if (cam == nullptr)
	{
		printf("DepthToRgbPass missing & requires a camera\n");
		return;
	}

	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);

	GLuint nearZLocation = glGetUniformLocation(shaderID, "nearZ");
	if (nearZLocation != -1)
		glUniform1f(nearZLocation, cam->getNearZ());
	GLuint farZLocation = glGetUniformLocation(shaderID, "farZ");
	if (farZLocation != -1)
		glUniform1f(farZLocation, cam->getFarZ());

	inputs[0]->bind(0);
}

void DepthToRgbPass::resize(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		if (!framebuffer->generate(width, height,
			{ { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA32F, nullptr } }))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);
}