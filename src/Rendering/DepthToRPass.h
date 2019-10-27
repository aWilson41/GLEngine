#pragma once
#include "RenderPass.h"

class ShaderProgram;

// Basically just copies over the depth into a new color buffer to render
class DepthToRPass : public RenderPass
{
public:
	DepthToRPass();
	~DepthToRPass();

public:
	GLuint* getColorOutput() { return &colorTexID; }

	void setDepthInput(GLuint* depthInput) { setInput(0, depthInput); }

	void render(DeferredRenderer* ren) override;
	void resizeFramebuffer(int width, int height) override;

private:
	ShaderProgram* shader = nullptr;
	GLuint colorTexID = -1;
};