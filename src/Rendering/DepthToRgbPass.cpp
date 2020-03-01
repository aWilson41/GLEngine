#include "DepthToRgbPass.h"
#include "Camera.h"
#include "DeferredRenderer.h"
#include "Framebuffer.h"
#include "Shaders.h"

DepthToRgbPass::DepthToRgbPass() : RenderPass("Depth_To_Rgb_Pass", RenderPassType::QUAD_PASS)
{
	shader = Shaders::loadVSFSShader("Depth_To_Rgb_Pass",
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

void DepthToRgbPass::bind(DeferredRenderer* ren)
{
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);

	GLuint nearZLocation = glGetUniformLocation(shaderID, "nearZ");
	if (nearZLocation != -1)
		glUniform1f(nearZLocation, ren->getCamera()->getNearZ());
	GLuint farZLocation = glGetUniformLocation(shaderID, "farZ");
	if (farZLocation != -1)
		glUniform1f(farZLocation, ren->getCamera()->getFarZ());

	inputs[0]->bind(0);
}

void DepthToRgbPass::resizeFramebuffer(UINT width, UINT height)
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

	*outputs[0] = *framebuffer->getAttachment(0);
}