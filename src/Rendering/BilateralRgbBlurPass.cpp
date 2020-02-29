#include "BilateralRgbBlurPass.h"
#include "DeferredRenderer.h"
#include "Framebuffer.h"
#include "Shaders.h"

BilateralRgbBlurPass::BilateralRgbBlurPass() : RenderPass("Bilateral_R_Blur_Pass")
{
	shader = Shaders::loadVSFSShader("Bilateral_R_Blur_Pass",
		"Shaders/DeferredRasterize/Passes/quadVS.glsl",
		"Shaders/DeferredRasterize/Passes/bilateralRgbBlurPass.glsl");
	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);
	glUniform1i(glGetUniformLocation(shaderID, "inputTex"), 0);
	glUseProgram(0);

	// Takes color and depth input and outputs color and depth
	setNumberOfInputPorts(1);
	setNumberOfOutputPorts(1);
}

void BilateralRgbBlurPass::render(DeferredRenderer* ren)
{
	// Use the default fbo to do the lighting pass
	framebuffer->bind();
	framebuffer->clearColor();

	GLuint shaderID = shader->getProgramID();
	glUseProgram(shaderID);

	// Set some uniforms
	GLuint blurRadiusLocation = glGetUniformLocation(shaderID, "blurRadius");
	if (blurRadiusLocation != -1)
		glUniform1i(blurRadiusLocation, blurRadius);
	GLuint sigmaILocation = glGetUniformLocation(shaderID, "sigmaI");
	if (sigmaILocation != -1)
		glUniform1f(sigmaILocation, sigmaI);
	GLuint sigmaSLocation = glGetUniformLocation(shaderID, "sigmaS");
	if (sigmaSLocation != -1)
		glUniform1f(sigmaSLocation, sigmaS);

	inputs[0]->bind(0);

	ren->quadPass();

	framebuffer->unbind();
}

void BilateralRgbBlurPass::resizeFramebuffer(UINT width, UINT height)
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