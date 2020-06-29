#pragma once
#include "FramePass.h"

class ShaderProgram;

class ComputeNormalsPass : public FramePass
{
public:
	ComputeNormalsPass();

public:
	std::shared_ptr<FramebufferAttachment> getNormalOutput() { return getOutput(0); }

	void setColorInput(std::shared_ptr<FramebufferAttachment> colorInput) { setInput(0, colorInput); }
	void setMaxDepth(float maxDepth) { this->maxDepth = maxDepth; }

	float getMaxDepth() const { return maxDepth; }

protected:
	void bindInputs() override;
	void resize(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;
	float maxDepth = 1000.0f;
};