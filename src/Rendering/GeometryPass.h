#pragma once
#include "CameraPass.h"

// Defines a RenderPass that renders the scene using a camera to a gbuffer of
// position, normal, diffuse, ambient, and depth
class GeometryPass : public CameraPass
{
public:
	GeometryPass(const std::string& passName = "Geometry_Pass");
	virtual ~GeometryPass() override = default;

public:
	std::shared_ptr<FramebufferAttachment> getPosOutput() const { return getOutput(0); }
	std::shared_ptr<FramebufferAttachment> getNormalOutput() const { return getOutput(1); }
	std::shared_ptr<FramebufferAttachment> getDiffuseOutput() const { return getOutput(2); }
	std::shared_ptr<FramebufferAttachment> getAmbientOutput() const { return getOutput(3); }
	std::shared_ptr<FramebufferAttachment> getDepthOutput() const { return getOutput(4); }

protected:
	void clear() override;
	void resize(UINT width, UINT height) override;
};