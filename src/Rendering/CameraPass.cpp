#include "CameraPass.h"
#include "AbstractMapper.h"
#include "Framebuffer.h"
#include "MapperComponent.h"
#include "Scene.h"
#include "SceneObject.h"

CameraPass::CameraPass(const std::string& passName) :
	RenderPass(passName)
{
	// Outputs color and depth
	setNumberOfInputPorts(0);
	setNumberOfOutputPorts(2);
}

void CameraPass::render(const std::string& shaderGroup)
{
	if (cam == nullptr || scene == nullptr)
	{
		printf("Scene or cam nullptr\n");
		return;
	}

	if (useDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	framebuffer->bind();

	// Clear the framebuffer, done differently for different passes
	clear();

	// Bind the inputs/fbos as textures
	bindInputs();

	// Pass over the entire scene
	for (Scene::iterator i = scene->begin(); i != scene->end(); i++)
	{
		std::shared_ptr<MapperComponent> mapperComp = (*i)->GetComponent<MapperComponent>();
		if (mapperComp != nullptr)
		{
			std::shared_ptr<AbstractMapper> mapper = mapperComp->getMapper();
			if (mapper->use(shaderGroup))
				mapper->draw(cam, scene);
		}
	}

	framebuffer->unbind();
}

void CameraPass::resize(UINT width, UINT height)
{
	// If it doesn't exist generate it
	if (!framebuffer->isGenerated())
	{
		std::vector<Framebuffer::AttachmentConfig> attachments;
		attachments.push_back({ Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr });
		if (useDepth)
			attachments.push_back({ Framebuffer::AttachmentType::DEPTH, FramebufferAttachment::Format::DEPTH32F, nullptr });
		
		if (!framebuffer->generate(width, height, attachments))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);
}