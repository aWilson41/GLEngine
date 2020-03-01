#pragma once
#include "MathHelper.h"
#include "PropertyMap.h"
#include <string>
#include <memory>

class AbstractMapper;
class Camera;
class Framebuffer;
class FramebufferAttachment;
class ImageData;

// Does the rendering, mostly just managing the scene (there is no scene object)
class Renderer
{
public:
	Renderer();

public:
	std::shared_ptr<AbstractMapper> getRenderItem(UINT i) const { return mappers[i]; }
	std::shared_ptr<Camera> getCamera() const { return cam; }
	std::shared_ptr<ImageData> getOutputImage() const;
	std::shared_ptr<Framebuffer> getColorOutput() const;
	std::shared_ptr<Framebuffer> getDepthOutput() const;
	std::shared_ptr<Framebuffer> getStencilOutput() const;
	glm::vec4 getClearColor() const { return clearColor; }
	glm::vec3 getLightDir() const { return lightDir; }
	glm::ivec2 getFramebufferDim() const;
	std::string getShaderGroup() const { return shaderGroup; };
	// Returns if renderer contains the mapper
	bool containsRenderItem(std::shared_ptr<AbstractMapper> mapper) const;

	void setCamera(std::shared_ptr<Camera> cam) { Renderer::cam = cam; }
	void setClearColor(float r, float g, float b, float a);
	void setShaderGroup(std::string shaderGroup) { Renderer::shaderGroup = shaderGroup; }
	void setDepthTest(bool depthTestOn) { depthTestOn ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST); }
	// Might split mapper into actor where this becomes addActor
	void addRenderItem(std::shared_ptr<AbstractMapper> mapper) { mappers.push_back(mapper); }

	virtual void render();
	// The direct renderer uses the default framebuffer
	virtual void resizeFramebuffer(UINT width, UINT height);

protected:
	// Default fbo of this renderer
	std::shared_ptr<Framebuffer> framebuffer;

	// Will eventually hold actors instead of mappers
	std::vector<std::shared_ptr<AbstractMapper>> mappers;
	std::shared_ptr<Camera> cam = nullptr;

	PropertyMap<32> sceneProperties;
	std::string shaderGroup = "";
	glm::vec3 lightDir = glm::vec3(0.0f, 1.0f, 1.0f); // Temporarily only supporting a single directional light

	glm::vec4 clearColor = glm::vec4(0.5f, 0.3f, 0.25f, 1.0f);

	std::shared_ptr<Framebuffer> colorOutputFbo = nullptr;
	std::shared_ptr<Framebuffer> depthOutputFbo = nullptr;
	std::shared_ptr<Framebuffer> stencilOutputFbo = nullptr;
};