#include "BilateralRgbBlurPass.h"
#include "Framebuffer.h"
#include "Shaders.h"

BilateralRgbBlurPass::BilateralRgbBlurPass() :
	FramePass("BilateralRBlur_Pass") 
{
	shader = Shaders::loadVSFSShader(passName,
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

void BilateralRgbBlurPass::bindInputs()
{
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
}

void BilateralRgbBlurPass::resize(UINT width, UINT height)
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
}