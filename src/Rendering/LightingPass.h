#pragma once
#include "RenderPass.h"

class ShaderProgram;

// Implements a basic phong lighting pass given a gbuffer with
// position, normal, diffuse, ambient, and depth
class LightingPass : public RenderPass
{
public:
	LightingPass();

public:
	std::shared_ptr<FramebufferAttachment> getColorOutput() { return getOutput(0); }

	void setPosInput(std::shared_ptr<FramebufferAttachment> posInput) { setInput(0, posInput); }
	void setNormalInput(std::shared_ptr<FramebufferAttachment> normalInput) { setInput(1, normalInput); }
	void setDiffuseInput(std::shared_ptr<FramebufferAttachment> diffuseInput) { setInput(2, diffuseInput); }
	void setAmbientInput(std::shared_ptr<FramebufferAttachment> ambientInput) { setInput(3, ambientInput); }

	void render(DeferredRenderer* ren) override;
	void resizeFramebuffer(UINT width, UINT height) override;

private:
	std::shared_ptr<ShaderProgram> shader = nullptr;
};