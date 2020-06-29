#pragma once
#include "Renderer.h"

class Framebuffer;
class RenderPass;

// Implements a deferred rendering process
// While Renderer defines a single camera pass
// The DeferredRenderer defines a vector passes that can be modified
// or the default GeometryPass -> LightingPass will be used
class DeferredRenderer : public Renderer
{
public:
	DeferredRenderer(bool useDefaults = true);
	virtual ~DeferredRenderer() override = default;

public:
	void setColorFbo(std::shared_ptr<Framebuffer> fbo) { colorOutputFbo = fbo; }
	void setDepthFbo(std::shared_ptr<Framebuffer> fbo) { depthOutputFbo = fbo; }
	void setStencilFbo(std::shared_ptr<Framebuffer> fbo) { stencilOutputFbo = fbo; }

	virtual void render() override;

	void addPass(std::shared_ptr<RenderPass> pass)
	{
		renderPasses.push_back(pass);
		PassesModified = true;
	}
	void removePass(std::shared_ptr<RenderPass> pass);

	void resizeFramebuffer(UINT width, UINT height) override;

protected:
	bool useDefaults = true;
	std::vector<std::shared_ptr<RenderPass>> renderPasses;
	bool PassesModified = false;
};