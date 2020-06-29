#pragma once
#include "FramePass.h"

class ShaderProgram;

class BilateralRBlurPass : public FramePass
{
public:
	BilateralRBlurPass();

public:
	std::shared_ptr<FramebufferAttachment> getColorOutput() { return getOutput(0); }

	void setColorInput(std::shared_ptr<FramebufferAttachment> colorInput) { setInput(0, colorInput); }
	// Area of effect (radius for the blur)
	void setRadius(UINT radius) { blurRadius = radius; }
	void SetSigmaI(GLfloat sigma) { sigmaI = sigma; }
	void SetSigmaS(GLfloat sigma) { sigmaS = sigma; }

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