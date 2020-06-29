#pragma once
#include "MathHelper.h"
#include "PropertyMap.h"
#include <string>
#include <memory>

class Scene;
class Camera;
class CameraPass;
class Framebuffer;
class ImageData;

// A renderer defines how to render a scene, this generally involves defining
// passes to utlimately produce a output color fbo
class Renderer
{
public:
	Renderer();
	virtual ~Renderer() = default;

public:
	std::shared_ptr<Scene> getScene() const { return scene; }
	std::shared_ptr<Camera> getCamera() const { return cam; }
	// Somewhat expensive copy operation, meant for outputting to files, or given to filters
	std::shared_ptr<ImageData> getOutputImage() const;
	std::shared_ptr<Framebuffer> getColorOutput() const;
	std::shared_ptr<Framebuffer> getDepthOutput() const;
	std::shared_ptr<Framebuffer> getStencilOutput() const;
	glm::vec4 getClearColor() const { return clearColor; }
	glm::ivec2 getFramebufferDim() const;
	std::string getShaderGroup() const { return shaderGroup; };

	void setScene(std::shared_ptr<Scene> scene) { this->scene = scene; }
	void setCamera(std::shared_ptr<Camera> cam) { this->cam = cam; }
	void setClearColor(float r, float g, float b, float a);
	void setShaderGroup(std::string shaderGroup) { this->shaderGroup = shaderGroup; }
	void setDepthTest(bool depthTestOn);

	virtual void render();
	// The direct renderer uses the default framebuffer
	virtual void resizeFramebuffer(UINT width, UINT height);

protected:
	// The scene to be rendered
	std::shared_ptr<Scene> scene = nullptr;

	// The camera to render with
	std::shared_ptr<Camera> cam = nullptr;

	// Defines the set of shaders to use
	std::string shaderGroup = "";

	glm::vec4 clearColor = glm::vec4(0.5f, 0.3f, 0.25f, 1.0f);

	std::shared_ptr<CameraPass> camPass = nullptr;
	std::shared_ptr<Framebuffer> colorOutputFbo = nullptr;
	std::shared_ptr<Framebuffer> depthOutputFbo = nullptr;
	std::shared_ptr<Framebuffer> stencilOutputFbo = nullptr;
};