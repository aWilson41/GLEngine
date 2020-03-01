#include "ComputeNormalsPass.h"
#include "Camera.h"
#include "DeferredRenderer.h"
#include "Framebuffer.h"
#include "Shaders.h"

ComputeNormalsPass::ComputeNormalsPass() : RenderPass("Compute_Normals_Pass", RenderPassType::QUAD_PASS)
{
	shader = Shaders::loadVSFSShader("Compute_Normals_Pass",
		"Shaders/DeferredRasterize/Passes/quadVS.glsl",
		"Shaders/DeferredRasterize/Passes/computeNormalsPass.glsl");
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);
	glUniform1i(glGetUniformLocation(shaderID, "inputTex"), 0);
	glUseProgram(0);

	// Takes color and depth input and outputs color and depth
	setNumberOfInputPorts(1);
	setNumberOfOutputPorts(1);
}

void ComputeNormalsPass::bind(DeferredRenderer* ren)
{
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);

	// Set some uniforms
	GLuint maxDepthLocation = glGetUniformLocation(shaderID, "maxDepth");
	if (maxDepthLocation != -1)
		glUniform1f(maxDepthLocation, maxDepth);
	GLuint invProjLocation = glGetUniformLocation(shaderID, "invProj");
	if (invProjLocation != -1)
	{
		glm::mat4 invProj = glm::inverse(ren->getCamera()->proj);
		glUniformMatrix4fv(invProjLocation, 1, GL_FALSE, &invProj[0][0]);
	}
	GLuint invViewLocation = glGetUniformLocation(shaderID, "invView");
	if (invViewLocation != -1)
	{
		glm::mat4 invView = glm::inverse(ren->getCamera()->view);
		glUniformMatrix4fv(invViewLocation, 1, GL_FALSE, &invView[0][0]);
	}
	GLuint texelSizeLocation = glGetUniformLocation(shaderID, "texelSize");
	if (texelSizeLocation != -1)
		glUniform2f(texelSizeLocation, 1.0f / fboWidth, 1.0f / fboHeight);
	GLuint nearZLocation = glGetUniformLocation(shaderID, "nearZ");
	if (nearZLocation != -1)
		glUniform1f(nearZLocation, ren->getCamera()->getNearZ());
	GLuint farZLocation = glGetUniformLocation(shaderID, "farZ");
	if (farZLocation != -1)
		glUniform1f(farZLocation, ren->getCamera()->getFarZ());

	inputs[0]->bind(0);
}

void ComputeNormalsPass::resizeFramebuffer(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		if (!framebuffer->generate(width, height,
			{ { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGB32F, nullptr } }))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);

	*outputs[0] = *framebuffer->getAttachment(0);
}