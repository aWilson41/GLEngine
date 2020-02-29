#pragma once
#include "RenderPass.h"

class GeometryPass : public RenderPass
{
public:
	GeometryPass();

public:
	std::shared_ptr<FramebufferAttachment> getPosOutput() const { return getOutput(0); }
	std::shared_ptr<FramebufferAttachment> getNormalOutput() const { return getOutput(1); }
	std::shared_ptr<FramebufferAttachment> getDiffuseOutput() const { return getOutput(2); }
	std::shared_ptr<FramebufferAttachment> getAmbientOutput() const { return getOutput(3); }
	std::shared_ptr<FramebufferAttachment> getDepthOutput() const { return getOutput(4); }

	void render(DeferredRenderer* ren) override;
	void resizeFramebuffer(UINT width, UINT height) override;
};