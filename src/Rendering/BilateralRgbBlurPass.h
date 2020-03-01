#pragma once
#include "RenderPass.h"

class ShaderProgram;

class BilateralRgbBlurPass : public RenderPass
{
public:
	BilateralRgbBlurPass();

public:
	std::shared_ptr<FramebufferAttachment> getColorOutput() const { return getOutput(0); }

	void setColorInput(std::shared_ptr<FramebufferAttachment> colorInput) { setInput(0, colorInput); }
	// Area of effect (radius for the blur)
	void setRadius(const GLuint radius) { blurRadius = radius; }
	void SetSigmaI(const GLfloat sigma) { sigmaI = sigma; }
	void SetSigmaS(const GLfloat sigma) { sigmaS = sigma; }

	void bindInputs(DeferredRenderer* ren) override;
	void resize(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;

	GLuint blurRadius = 15;
	// Intensity domain
	GLfloat sigmaI = 0.2f;
	// Spatial domain, increase to make positional differences more important
	GLfloat sigmaS = 1.0f;
};