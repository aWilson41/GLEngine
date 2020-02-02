#pragma once
#include "MathHelper.h"
#include "PropertyMap.h"
#include <string>
#include <memory>

class AbstractMapper;
class Camera;
class ImageData;

// Does the rendering, mostly just managing the scene (there is no scene object)
class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	std::shared_ptr<AbstractMapper> getRenderItem(UINT i) const { return mappers[i]; }
	std::shared_ptr<Camera> getCamera() const { return cam; }
	ImageData* getOutputImage() const;
	float* getClearColor() { return clearColor; }
	glm::vec3 getLightDir() const { return lightDir; }
	int getFramebufferWidth() const { return defaultFboWidth; }
	int getFramebufferHeight() const { return defaultFboHeight; }
	std::string getShaderGroup() const { return shaderGroup; };
	// Returns if renderer contains the mapper
	bool containsRenderItem(std::shared_ptr<AbstractMapper> mapper) const;

	void setCamera(std::shared_ptr<Camera> cam) { Renderer::cam = cam; }
	void setClearColor(float r, float g, float b, float a);
	void setShaderGroup(std::string shaderGroup) { Renderer::shaderGroup = shaderGroup; }
	// Might split mapper into actor where this becomes addActor
	void addRenderItem(std::shared_ptr<AbstractMapper> mapper) { mappers.push_back(mapper); }

	virtual void render();
	// The direct renderer uses the default framebuffer
	virtual void resizeFramebuffer(int width, int height);

protected:
	// Will eventually hold actors instead of mappers
	std::vector<std::shared_ptr<AbstractMapper>> mappers;
	std::shared_ptr<Camera> cam = nullptr;
	bool initialized = false;

	PropertyMap<32> sceneProperties;
	std::string shaderGroup = "";
	glm::vec3 lightDir = glm::vec3(0.0f, 1.0f, 1.0f); // Temporarily only supporting a single directional light

	// Default framebuffer size, resized on init
	int defaultFboWidth = 100;
	int defaultFboHeight = 100;
	float clearColor[4] = { 0.5f, 0.3f, 0.25f, 1.0f };
};