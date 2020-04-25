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
	virtual ~DeferredRenderer() override;

public:
	void setColorFbo(std::shared_ptr<Framebuffer> fbo) { colorOutputFbo = fbo; }
	void setDepthFbo(std::shared_ptr<Framebuffer> fbo) { depthOutputFbo = fbo; }
	void setStencilFbo(std::shared_ptr<Framebuffer> fbo) { stencilOutputFbo = fbo; }

	void render() override;

	// Does a full pass over every mapper
	void fullPass();
	// Does a pass with a quad
	void quadPass();

	void addPass(std::shared_ptr<RenderPass> pass)
	{
		renderPasses.push_back(pass);
		PassesModified = true;
	}
	void removePass(std::shared_ptr<RenderPass> pass);

	void resizeFramebuffer(UINT width, UINT height) override;

private:
	GLuint emptyVaoID = -1;
	bool useDefaults = true;
	std::vector<std::shared_ptr<RenderPass>> renderPasses;
	bool PassesModified = false;
};