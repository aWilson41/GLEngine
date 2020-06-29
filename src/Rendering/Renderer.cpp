#include "Renderer.h"
#include "AbstractMapper.h"
#include "CameraPass.h"
#include "Framebuffer.h"
#include "ImageData.h"
#include "MapperComponent.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Shaders.h"

Renderer::Renderer() :
	camPass(std::make_shared<CameraPass>()),
	scene(std::make_shared<Scene>())
{
	shaderGroup = "DirectRasterize";
	colorOutputFbo = depthOutputFbo = stencilOutputFbo = camPass->getFramebuffer();
	camPass->setScene(scene);
}

std::shared_ptr<ImageData> Renderer::getOutputImage() const
{
	const glm::ivec2 fboDim = getFramebufferDim();
	// Bind the default fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	std::shared_ptr<ImageData> results = std::make_shared<ImageData>();
	UINT dim[2] = { static_cast<UINT>(fboDim.x), static_cast<UINT>(fboDim.y) };
	double spacing[2] = { 1.0, 1.0 };
	double origin[2] = { 0.0, 0.0 };
	results->allocate2DImage(dim, spacing, origin, 4, ScalarType::UCHAR_T);

	unsigned char* buffer = new unsigned char[dim[0] * dim[1] * 4u];
	glReadPixels(0, 0, fboDim.x, fboDim.y, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	unsigned char* imgPtr = static_cast<unsigned char*>(results->getData());
	// Flip the image
	for (UINT y = 0; y < dim[1]; y++)
	{
		for (UINT x = 0; x < dim[0]; x++)
		{
			UINT index1 = (y * fboDim.x + x) * 4;
			UINT index2 = (((fboDim.y - 1) - y) * fboDim.x + x) * 4;
			imgPtr[index1] = buffer[index2];
			imgPtr[index1 + 1] = buffer[index2 + 1];
			imgPtr[index1 + 2] = buffer[index2 + 2];
			imgPtr[index1 + 3] = buffer[index2 + 3];
		}
	}
	delete[] buffer;
	return results;
}

std::shared_ptr<Framebuffer> Renderer::getColorOutput() const { return colorOutputFbo; }
std::shared_ptr<Framebuffer> Renderer::getDepthOutput() const { return depthOutputFbo; }
std::shared_ptr<Framebuffer> Renderer::getStencilOutput() const { return stencilOutputFbo; }

glm::ivec2 Renderer::getFramebufferDim() const { return colorOutputFbo->getDim(); }

void Renderer::setClearColor(float r, float g, float b, float a)
{
	clearColor = glm::vec4(r, g, b, a);
	glClearColor(r, g, b, a);
}
void Renderer::setDepthTest(bool depthTestOn) { camPass->setUseDepth(depthTestOn); }

void Renderer::render()
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

	camPass->setScene(scene);
	camPass->setCamera(cam);
	camPass->render(shaderGroup);
}

void Renderer::resizeFramebuffer(UINT width, UINT height)
{
	camPass->resizeFramebuffer(width, height);

	glViewport(0, 0, width, height);
}