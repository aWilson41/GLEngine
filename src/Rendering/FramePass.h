#pragma once
#include "RenderPass.h"

class Camera;

// The frame pass defines an RenderPass operation over a framebuffer itself
// using a quad pass
class FramePass : public RenderPass
{
protected:
	FramePass(const std::string& passName);

public:
	virtual ~FramePass() override;

public:
	virtual void render(const std::string& shaderGroup) override;

protected:
	UINT emptyVaoID = -1;
};