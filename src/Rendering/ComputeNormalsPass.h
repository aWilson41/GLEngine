#pragma once
#include "RenderPass.h"

class ShaderProgram;

class ComputeNormalsPass : public RenderPass
{
public:
	ComputeNormalsPass();

public:
	std::shared_ptr<FramebufferAttachment> getNormalOutput() { return getOutput(0); }

	void setColorInput(std::shared_ptr<FramebufferAttachment> colorInput) { setInput(0, colorInput); }
	void setMaxDepth(GLfloat maxDepth) { ComputeNormalsPass::maxDepth = maxDepth; }

	void bindInputs(DeferredRenderer* ren) override;
	void resize(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;
	GLfloat maxDepth = 1000.0f;
};