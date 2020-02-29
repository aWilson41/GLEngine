#pragma once
#include "MathHelper.h"
#include <string>
#include <memory>

class DeferredRenderer;
class Framebuffer;
class FramebufferAttachment;

class RenderPass
{
public:
	RenderPass(std::string name)
	{
		passName = name;
		framebuffer = std::make_shared<Framebuffer>();
	}

public:
	UINT getNumberOfInputPorts() const { return static_cast<UINT>(inputs.size()); }
	UINT getNumberOfOutputPorts() const { return static_cast<UINT>(outputs.size()); }
	std::shared_ptr<FramebufferAttachment> getInput(size_t portNum) const { return inputs[portNum]; };
	std::shared_ptr<FramebufferAttachment> getOutput(size_t portNum) const { return outputs[portNum]; }
	std::string getPassName() const { return passName; }
	std::shared_ptr<Framebuffer> getFramebuffer() const { return framebuffer; }

	void setInput(size_t portNum, std::shared_ptr<FramebufferAttachment> fboIn) { inputs[portNum] = fboIn; }
	void setNumberOfInputPorts(UINT numberOfPorts)
	{
		inputs.resize(numberOfPorts);
		std::fill_n(inputs.data(), inputs.size(), nullptr);
	}
	void setNumberOfOutputPorts(UINT numberOfPorts);

	virtual void render(DeferredRenderer* ren) = 0;
	virtual void resizeFramebuffer(UINT width, UINT height) = 0;

protected:
	std::string passName = "Unnamed";
	std::shared_ptr<Framebuffer> framebuffer = nullptr;
	std::vector<std::shared_ptr<FramebufferAttachment>> inputs;
	std::vector<std::shared_ptr<FramebufferAttachment>> outputs;
	UINT fboWidth = 100;
	UINT fboHeight = 100;
};