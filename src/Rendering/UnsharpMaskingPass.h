#pragma once
#include "FramePass.h"

class ShaderProgram;

// This pass provides a cheap AO trick.
// From "Image Enhancement by Unsharp Masking the Depth Buffer; Luft, et al. Siggraph 2006
// We essentially blur the depth buffer and subtract it from the original depth buffer
// Then blend into the color.
class UnsharpMaskingPass : public FramePass
{
public:
	UnsharpMaskingPass();

public:
	void setColorInput(std::shared_ptr<FramebufferAttachment> colorInput) { setInput(0, colorInput); }
	void setDepthInput(std::shared_ptr<FramebufferAttachment> depthInput) { setInput(1, depthInput); }
	// Enhances the effect
	void setDarknessFactor(float darknessFactor) { this->darknessFactor = darknessFactor; }
	// Area of effect (radius for the gaussian blur)
	void setRadius(UINT radius) { this->radius = radius; }
	// If specified will use a ratio of the buffer diagonal instead
	void setRadiusRatio(float radiusRatio) { this->radiusRatio = radiusRatio; }
	// Gaussian blur sigma, if not specified will use sigma = blurRadius
	void setSigma(float sigma) { this->sigma = sigma; }

	float getDarknessFactor() const { return darknessFactor; }
	UINT getRadius() const { return radius; }
	float getRadiusRatio() const { return radiusRatio; }
	float getSigma() const { return sigma; }

protected:
	void bindInputs() override;
	void resize(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;

	GLfloat darknessFactor = 5.0f;
	GLuint radius = 20;
	GLfloat radiusRatio = -1.0f;
	GLfloat sigma = 2.0f;
};