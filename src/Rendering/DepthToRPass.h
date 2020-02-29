#pragma once
#include "RenderPass.h"

class ShaderProgram;

// Basically just copies over the depth into a new color buffer to render
class DepthToRPass : public RenderPass
{
public:
	DepthToRPass();

public:
	std::shared_ptr<FramebufferAttachment> getColorOutput() { return getOutput(0); }

	void setDepthInput(std::shared_ptr<FramebufferAttachment> depthInput) { setInput(0, depthInput); }

	void render(DeferredRenderer* ren) override;
	void resizeFramebuffer(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;
};