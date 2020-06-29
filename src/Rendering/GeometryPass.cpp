#include "GeometryPass.h"
#include "Framebuffer.h"
#include "Shaders.h"

GeometryPass::GeometryPass(const std::string& passName) :
	CameraPass(passName)
{
	setNumberOfInputPorts(0);
	setNumberOfOutputPorts(5);
}

void GeometryPass::clear()
{
	framebuffer->getAttachment(0)->clearTex(); // Position buffer
	framebuffer->getAttachment(1)->clearTex(); // Normal buffer
	framebuffer->getAttachment(2)->clearTex(); // Diffuse buffer
	//const glm::vec4 clearColor = ren->getClearColor();
	//framebuffer->getAttachment(3)->clearTex( // Ambient buffer
	//	static_cast<unsigned char>(clearColor.r * 255.0f), 
	//	static_cast<unsigned char>(clearColor.g * 255.0f),
	//	static_cast<unsigned char>(clearColor.b * 255.0f),
	//	static_cast<unsigned char>(clearColor.a * 255.0f));
	framebuffer->getAttachment(3)->clearTex( // Ambient buffer
		static_cast<unsigned char>(0.5f * 255.0f),
		static_cast<unsigned char>(0.3f * 255.0f),
		static_cast<unsigned char>(0.25f * 255.0f),
		static_cast<unsigned char>(255.0f));
	framebuffer->getAttachment(4)->clearTex(); // Depth buffer
}

void GeometryPass::resize(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		std::vector<Framebuffer::AttachmentConfig> attachments(4);
		attachments[0] = { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGB16F, nullptr };
		attachments[1] = { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGB16F, nullptr };
		attachments[2] = { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr };
		attachments[3] = { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr };
		if (useDepth)
			attachments.push_back({ Framebuffer::AttachmentType::DEPTH, FramebufferAttachment::Format::DEPTH32F, nullptr });

		if (!framebuffer->generate(width, height, attachments))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);
}