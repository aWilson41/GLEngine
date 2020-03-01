#include "GeometryPass.h"
#include "DeferredRenderer.h"
#include "Framebuffer.h"
#include "Shaders.h"

GeometryPass::GeometryPass() : RenderPass("Geometry Pass", RenderPassType::FULL_PASS)
{
	setNumberOfInputPorts(0);
	setNumberOfOutputPorts(5);
}

void GeometryPass::clear(DeferredRenderer* ren)
{
	framebuffer->getAttachment(0)->clearTex(); // Position buffer
	framebuffer->getAttachment(1)->clearTex(); // Normal buffer
	framebuffer->getAttachment(2)->clearTex(); // Diffuse buffer
	const glm::vec4 clearColor = ren->getClearColor();
	framebuffer->getAttachment(3)->clearTex( // Ambient buffer
		static_cast<unsigned char>(clearColor.r * 255.0f), 
		static_cast<unsigned char>(clearColor.g * 255.0f),
		static_cast<unsigned char>(clearColor.b * 255.0f),
		static_cast<unsigned char>(clearColor.a * 255.0f));
	framebuffer->getAttachment(4)->clearTex(); // Depth buffer
}

void GeometryPass::resize(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		if (!framebuffer->generate(width, height, {
			{ Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGB16F, nullptr },
			{ Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGB16F, nullptr },
			{ Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr },
			{ Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr },
			{ Framebuffer::AttachmentType::DEPTH, FramebufferAttachment::Format::DEPTH32F, nullptr } }))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);
}