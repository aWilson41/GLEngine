#pragma once
#include "MathHelper.h"
#include <string>
#include <memory>

class AbstractMapper;
class DeferredRenderer;
class Framebuffer;
class FramebufferAttachment;

enum class RenderPassType
{
	FULL_PASS,
	QUAD_PASS,
	CUSTOM_PASS
};

class RenderPass
{
public:
	RenderPass(std::string name, RenderPassType type) : 
		passName(name), passType(type)
	{
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
	// If on the pass will iterate over
	void setPassType(const RenderPassType passType) { this->passType = passType; }

	void addMapper(std::shared_ptr<AbstractMapper> mapper) { mappers.push_back(mapper); }

	void resizeFramebuffer(UINT width, UINT height);
	void render(DeferredRenderer* ren);

protected:
	virtual void clear(DeferredRenderer* ren);
	virtual void bindInputs(DeferredRenderer* ren) { };
	virtual void resize(UINT width, UINT height) = 0;

protected:
	std::string passName = "Unnamed";
	std::shared_ptr<Framebuffer> framebuffer = nullptr;
	std::vector<std::shared_ptr<FramebufferAttachment>> inputs;
	std::vector<std::shared_ptr<FramebufferAttachment>> outputs;
	UINT fboWidth = 100;
	UINT fboHeight = 100;

	RenderPassType passType = RenderPassType::FULL_PASS;
	std::vector<std::shared_ptr<AbstractMapper>> mappers;
};