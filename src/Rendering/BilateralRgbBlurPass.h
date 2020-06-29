#pragma once
#include "FramePass.h"

class ShaderProgram;

class BilateralRgbBlurPass : public FramePass
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

protected:
	void bindInputs() override;
	void resize(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;

	GLuint blurRadius = 15;
	// Intensity domain
	GLfloat sigmaI = 0.2f;
	// Spatial domain, increase to make positional differences more important
	GLfloat sigmaS = 1.0f;
};