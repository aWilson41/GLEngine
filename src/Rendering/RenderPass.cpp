#include "RenderPass.h"
#include "Framebuffer.h"

void RenderPass::setNumberOfOutputPorts(UINT numberOfPorts)
{
	outputs = std::vector<std::shared_ptr<FramebufferAttachment>>(numberOfPorts);
	for (UINT i = 0; i < outputs.size(); i++)
	{
		outputs[i] = std::make_shared<FramebufferAttachment>();
	}
}