#pragma once
#include "FramePass.h"

class ShaderProgram;

// Basically just copies over the depth into a new color buffer to render
class DepthToRgbPass : public FramePass
{
public:
	DepthToRgbPass();

public:
	std::shared_ptr<FramebufferAttachment> getColorOutput() const { return getOutput(0); }

	void setDepthInput(std::shared_ptr<FramebufferAttachment> depthInput) { setInput(0, depthInput); }

protected:
	void bindInputs() override;
	void resize(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;
	std::shared_ptr<Camera> cam = nullptr;
};