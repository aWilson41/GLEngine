#pragma once
#include "AbstractMapper.h"
#include <memory>

class ImageData;
class PlaneSource;
class ShaderProgram;

// Does rendering and pre rendering operations (mapping input to visual representation)
class ImageMapper : public AbstractMapper
{
public:
	ImageMapper();
	~ImageMapper();

public:
	std::shared_ptr<ImageData> getInput() const { return imageData; }
	glm::mat4 getModelMatrix() const { return model; }
	GLuint getShaderProgramID() const override;
	std::string getMapperName() const override { return "ImageMapper"; }

	void setInput(std::shared_ptr<ImageData> data) { imageData = data; }
	void setModelMatrix(glm::mat4 model) { ImageMapper::model = model; }

	// Updates the buffers to match the input data
	void update() override;

	bool useShader(std::string shaderGroup) override;
	void draw(Renderer* ren) const override;

protected:
	void updateBuffer();

protected:
	std::shared_ptr<ImageData> imageData = nullptr;
	std::shared_ptr<PlaneSource> planeSource = nullptr;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 imageSizeMat = glm::mat4(1.0f);

	GLuint vboID = -1;
	GLuint iboID = -1;
	GLuint vaoID = -1;
	GLuint texID = -1;

	ShaderProgram* shaderProgram = nullptr;
};