#pragma once
#include "RenderPass.h"

class Camera;
class Scene;

// Defines a RenderPass operation with no fbo inputs that produces
// a color and depth output. It renders a Scene with a Camera
// todo: add stencil buffer
class CameraPass : public RenderPass
{
public:
	CameraPass(const std::string& passName = "Camera_Pass");
	virtual ~CameraPass() override = default;

public:
	bool getUseDepth() const { return useDepth; }
	std::shared_ptr<FramebufferAttachment> getColorOutput() const { return getOutput(0); }
	std::shared_ptr<FramebufferAttachment> getDepthOutput() const { return getOutput(1); }

	// Set whether a depth buffer should be generated and used when rendering
	void setUseDepth(bool useDepth) { this->useDepth = useDepth; }

public:
	virtual void render(const std::string& shaderGroup) override;

protected:
	virtual void resize(UINT width, UINT height) override;

protected:
	bool useDepth = true;
};