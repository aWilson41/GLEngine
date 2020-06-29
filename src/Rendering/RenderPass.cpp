#include "RenderPass.h"
#include "AbstractMapper.h"
#include "DeferredRenderer.h"
#include "Framebuffer.h"

RenderPass::RenderPass(const std::string& name) :
	passName(name),
	framebuffer(std::make_shared<Framebuffer>())
{
}

void RenderPass::setNumberOfOutputPorts(UINT numberOfPorts)
{
	outputs = std::vector<std::shared_ptr<FramebufferAttachment>>(numberOfPorts);
	for (UINT i = 0; i < outputs.size(); i++)
	{
		outputs[i] = std::make_shared<FramebufferAttachment>();
	}
}

void RenderPass::clear() { framebuffer->clear(); }

void RenderPass::resizeFramebuffer(UINT width, UINT height)
{
	resize(width, height);

	// Copy the outputs
	const UINT numAttachments = framebuffer->getNumberOfAttachments();
	for (UINT i = 0; i < numAttachments; i++)
	{
		*outputs[i] = *framebuffer->getAttachment(i);
	}
}