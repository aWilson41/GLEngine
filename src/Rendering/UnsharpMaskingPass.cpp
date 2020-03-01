#include "UnsharpMaskingPass.h"
#include "DeferredRenderer.h"
#include "Framebuffer.h"
#include "Shaders.h"

UnsharpMaskingPass::UnsharpMaskingPass() : RenderPass("Unsharp Masking Pass", RenderPassType::QUAD_PASS)
{
	shader = Shaders::loadVSFSShader("Unsharp_Masking_Pass",
		"Shaders/DeferredRasterize/Passes/quadVS.glsl",
		"Shaders/DeferredRasterize/Passes/unsharpMaskingPass.glsl");
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);
	glUniform1i(glGetUniformLocation(shaderID, "colorTex"), 0);
	glUniform1i(glGetUniformLocation(shaderID, "depthTex"), 1);
	glUseProgram(0);

	// Takes color and depth input and outputs color and depth
	setNumberOfInputPorts(2);
	setNumberOfOutputPorts(1);
}

void UnsharpMaskingPass::bind(DeferredRenderer* ren)
{
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);

	// Set some uniforms
	GLuint blurRadiusLocation = glGetUniformLocation(shaderID, "blurRadius");
	if (blurRadiusLocation != -1) // 3% of the diagonal (paper cites 2%-5%)
	{
		if (radiusRatio != -1.0f)
			radius = static_cast<GLuint>(sqrt(fboWidth * fboHeight + fboWidth * fboHeight) * radiusRatio);
		glUniform1i(blurRadiusLocation, radius);
	}
	GLuint darknessFactorLocation = glGetUniformLocation(shaderID, "darknessFactor");
	if (darknessFactorLocation != -1)
		glUniform1f(darknessFactorLocation, darknessFactor);
	GLuint sigmaLocation = glGetUniformLocation(shaderID, "sigma");
	if (sigmaLocation != -1)
	{
		if (sigma == -1.0f)
			glUniform1f(sigmaLocation, static_cast<GLfloat>(radius));
		else
			glUniform1f(sigmaLocation, sigma);
	}

	inputs[0]->bind(0);
	inputs[1]->bind(1);
}

void UnsharpMaskingPass::resizeFramebuffer(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		if (!framebuffer->generate(width, height,
			{ { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr },
			{ Framebuffer::AttachmentType::DEPTH, FramebufferAttachment::Format::DEPTH32F, nullptr } }))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);

	*outputs[0] = *framebuffer->getAttachment(0);
	*outputs[1] = *framebuffer->getAttachment(1);
}