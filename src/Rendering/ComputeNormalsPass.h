#pragma once
#include "RenderPass.h"

class ShaderProgram;

class ComputeNormalsPass : public RenderPass
{
public:
	ComputeNormalsPass();
	~ComputeNormalsPass();

public:
	GLuint* getNormalOutput() { return &normalTexID; }

	void setColorInput(GLuint* colorInput) { setInput(0, colorInput); }
	void setMaxDepth(GLfloat maxDepth) { ComputeNormalsPass::maxDepth = maxDepth; }

	void render(DeferredRenderer* ren) override;
	void resizeFramebuffer(int width, int height) override;

private:
	ShaderProgram* shader = nullptr;
	GLuint normalTexID = -1;

	GLfloat maxDepth = 1000.0f;
};