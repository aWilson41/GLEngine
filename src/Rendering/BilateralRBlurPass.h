#pragma once
#include "RenderPass.h"

class ShaderProgram;

class BilateralRBlurPass : public RenderPass
{
public:
	BilateralRBlurPass();
	~BilateralRBlurPass();

public:
	GLuint* getColorOutput() { return &colorTexID; }

	void setColorInput(GLuint* colorInput) { setInput(0, colorInput); }
	// Area of effect (radius for the blur)
	void setRadius(GLuint radius) { blurRadius = radius; }
	void SetSigmaI(GLfloat sigma) { sigmaI = sigma; }
	void SetSigmaS(GLfloat sigma) { sigmaS = sigma; }

	void render(DeferredRenderer* ren) override;
	void resizeFramebuffer(int width, int height) override;

private:
	ShaderProgram* shader = nullptr;
	GLuint colorTexID = -1;

	GLuint blurRadius = 15;
	// Intensity domain
	GLfloat sigmaI = 0.2f;
	// Spatial domain, increase to make positional differences more important
	GLfloat sigmaS = 1.0f;
};