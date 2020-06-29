#include "DeferredRenderer.h"
#include "AbstractMapper.h"
#include "Framebuffer.h"
#include "GeometryPass.h"
#include "LightingPass.h"
#include "MapperComponent.h"
#include "PlaneSource.h"
#include "PolyDataMapper.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Shaders.h"

DeferredRenderer::DeferredRenderer(bool useDefaults)
{
	shaderGroup = "DeferredRasterize";

	this->useDefaults = useDefaults;
	if (useDefaults)
	{
		std::shared_ptr<GeometryPass> geomPass = std::make_shared<GeometryPass>();
		std::shared_ptr<LightingPass> lightPass = std::make_shared<LightingPass>();

		lightPass->setPosInput(geomPass->getPosOutput());
		lightPass->setNormalInput(geomPass->getNormalOutput());
		lightPass->setDiffuseInput(geomPass->getDiffuseOutput());
		lightPass->setAmbientInput(geomPass->getAmbientOutput());

		addPass(geomPass);
		addPass(lightPass);

		setColorFbo(lightPass->getFramebuffer());
		setDepthFbo(geomPass->getFramebuffer());
	}
}

void DeferredRenderer::render()
{
	if (cam == nullptr)
	{
		printf("Renderer missing camera.\n");
		return;
	}

	if (scene == nullptr)
	{
		printf("Renderer missing a scene.\n");
		return;
	}

	const glm::ivec2 fboDim = getFramebufferDim();
	if (PassesModified)
		resizeFramebuffer(fboDim.x, fboDim.y);

	// Execute all the render passses
	for (UINT i = 0; i < renderPasses.size(); i++)
	{
		renderPasses[i]->setScene(scene);
		renderPasses[i]->setCamera(cam);
		renderPasses[i]->render(shaderGroup);
	}
}

void DeferredRenderer::removePass(std::shared_ptr<RenderPass> pass)
{
	size_t passIndex = -1;
	for (size_t i = 0; i < renderPasses.size(); i++)
	{
		if (pass = renderPasses[i])
		{
			passIndex = i;
			break;
		}
	}
	if (passIndex == -1)
		return;
	renderPasses.erase(renderPasses.begin() + passIndex);
	PassesModified = true;
}

// Resizes the framebuffer (deletes and recreates), can also be used for initialization
void DeferredRenderer::resizeFramebuffer(UINT width, UINT height)
{
	Renderer::resizeFramebuffer(width, height);

	// Recursively resize framebuffers
	for (UINT i = 0; i < renderPasses.size(); i++)
	{
		renderPasses[i]->resizeFramebuffer(width, height);
	}
	PassesModified = false;
}