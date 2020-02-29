#pragma once
#include "Renderer.h"

class Framebuffer;
class RenderPass;

// Implements a deferred rendering process
// Specifically, it implements render passes which manage various framebuffers
class DeferredRenderer : public Renderer
{
public:
	DeferredRenderer(bool useDefaults = true);
	~DeferredRenderer();

public:
	void setColorFbo(std::shared_ptr<Framebuffer> fbo) { colorOutputFbo = fbo; }
	void setDepthFbo(std::shared_ptr<Framebuffer> fbo) { depthOutputFbo = fbo; }
	void setStencilFbo(std::shared_ptr<Framebuffer> fbo) { stencilOutputFbo = fbo; }

	void render() override;

	void pass();
	void quadPass();

	void addPass(RenderPass* pass)
	{
		renderPasses.push_back(pass);
		PassesModified = true;
	}
	void removePass(RenderPass* pass);

	void resizeFramebuffer(UINT width, UINT height) override;

private:
	GLuint emptyVaoID = -1;
	bool useDefaults = true;
	std::vector<RenderPass*> renderPasses;
	// The color and depth fbo's to blit after everything is rendered
	
	bool PassesModified = false;
};