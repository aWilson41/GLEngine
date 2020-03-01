#include "DeferredRenderer.h"
#include "AbstractMapper.h"
#include "Framebuffer.h"
#include "GeometryPass.h"
#include "LightingPass.h"
#include "PlaneSource.h"
#include "PolyDataMapper.h"
#include "Shaders.h"

DeferredRenderer::DeferredRenderer(bool useDefaults)
{
	shaderGroup = "DeferredRasterize";

	// A non-zero vao must be bound even if not using vertex attributes for quad pass
	glGenVertexArrays(1, &emptyVaoID);

	DeferredRenderer::useDefaults = useDefaults;
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

DeferredRenderer::~DeferredRenderer()
{
	glDeleteVertexArrays(1, &emptyVaoID);
}

void DeferredRenderer::render()
{
	const glm::ivec2 fboDim = getFramebufferDim();
	if (PassesModified)
		resizeFramebuffer(fboDim.x, fboDim.y);

	// Execute all the render passses
	for (UINT i = 0; i < renderPasses.size(); i++)
	{
		renderPasses[i]->render(this);
	}
}

void DeferredRenderer::fullPass()
{
	// Render the geometry
	for (UINT i = 0; i < mappers.size(); i++)
	{
		std::shared_ptr<AbstractMapper> mapper = mappers[i];
		mapper->use(this);
		mapper->draw(this);
	}
}

void DeferredRenderer::quadPass()
{
	// Then render the quad
	glBindVertexArray(emptyVaoID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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