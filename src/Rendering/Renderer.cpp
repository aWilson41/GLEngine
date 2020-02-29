#include "Renderer.h"
#include "AbstractMapper.h"
#include "Framebuffer.h"
#include "ImageData.h"
#include "Shaders.h"

Renderer::Renderer()
{
	shaderGroup = "DirectRasterize";
	framebuffer = colorOutputFbo = depthOutputFbo = stencilOutputFbo = std::make_shared<Framebuffer>();
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

glm::ivec2 Renderer::getFramebufferDim() const { return framebuffer->getDim(); }

bool Renderer::containsRenderItem(std::shared_ptr<AbstractMapper> mapper) const
{
	for (UINT i = 0; i < mappers.size(); i++)
	{
		if (mappers[i] == mapper)
			return true;
	}
	return false;
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
	clearColor[0] = r;
	clearColor[1] = g;
	clearColor[2] = b;
	clearColor[3] = a;
	glClearColor(r, g, b, a);
}

void Renderer::render()
{
	if (cam == nullptr)
		printf("Renderer missing camera.\n");

	framebuffer->bind();
	framebuffer->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (UINT i = 0; i < mappers.size(); i++)
	{
		std::shared_ptr<AbstractMapper> mapper = mappers[i];
		if (mapper->use(this))
		{
			// Set the scene uniforms
			GLuint lightDirLocation = glGetUniformLocation(mapper->getShaderProgramID(), "lightDir");
			if (lightDirLocation != -1)
				glUniform3fv(lightDirLocation, 1, &lightDir[0]);

			mapper->draw(this);
		}
	}

	framebuffer->unbind();
}

void Renderer::resizeFramebuffer(UINT width, UINT height)
{
	if (!framebuffer->isGenerated())
	{
		if (!framebuffer->generate(width, height,
			{ { Framebuffer::AttachmentType::COLOR, FramebufferAttachment::Format::RGBA, nullptr },
			{ Framebuffer::AttachmentType::DEPTH, FramebufferAttachment::Format::DEPTH32F, nullptr } }))
			printf("Warning, framebuffer incomplete");
	}
	else
		framebuffer->resize(width, height);

	glViewport(0, 0, width, height);
}