#pragma once
#include "MathHelper.h"
#include <string>
#include <memory>

class AbstractMapper;
class Camera;
class Framebuffer;
class FramebufferAttachment;
class Scene;

// A render pass takes 0-N input framebuffers and produces
// 0-N output framebuffers by doing rendering
class RenderPass
{
protected:
	RenderPass(const std::string& name);

public:
	virtual ~RenderPass() = default;

public:
	UINT getNumberOfInputPorts() const { return static_cast<UINT>(inputs.size()); }
	UINT getNumberOfOutputPorts() const { return static_cast<UINT>(outputs.size()); }
	std::shared_ptr<FramebufferAttachment> getInput(size_t portNum) const { return inputs[portNum]; };
	std::shared_ptr<FramebufferAttachment> getOutput(size_t portNum) const { return outputs[portNum]; }
	const std::string& getPassName() const { return passName; }
	std::shared_ptr<Framebuffer> getFramebuffer() const { return framebuffer; }
	std::shared_ptr<Camera> getCamera() const { return cam; }
	std::shared_ptr<Scene> getScene() const { return scene; }

	void setScene(std::shared_ptr<Scene> scene) { this->scene = scene; }
	void setCamera(std::shared_ptr<Camera> cam) { this->cam = cam; }
	void setInput(size_t portNum, std::shared_ptr<FramebufferAttachment> fboIn) { inputs[portNum] = fboIn; }
	void setNumberOfInputPorts(UINT numberOfPorts)
	{
		inputs.resize(numberOfPorts);
		std::fill_n(inputs.data(), inputs.size(), nullptr);
	}
	void setNumberOfOutputPorts(UINT numberOfPorts);

	void resizeFramebuffer(UINT width, UINT height);
	virtual void render(const std::string& shaderGroup) = 0;

protected:
	// Calls glClear, one may override to clear to more specific values
	virtual void clear();
	virtual void bindInputs() { };
	// Resize is also responsible for defining the fbo attachments
	virtual void resize(UINT width, UINT height) = 0;

protected:
	std::string passName = "Unnamed";
	std::shared_ptr<Framebuffer> framebuffer = nullptr;
	std::vector<std::shared_ptr<FramebufferAttachment>> inputs;
	std::vector<std::shared_ptr<FramebufferAttachment>> outputs;
	std::shared_ptr<Scene> scene = nullptr;
	std::shared_ptr<Camera> cam = nullptr;
	UINT fboWidth = 100;
	UINT fboHeight = 100;
};