#pragma once
#include "AbstractMapper.h"

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
	ImageData* getInput() { return imageData; }
	glm::mat4 getModelMatrix() { return model; }

	void setInput(ImageData* data);
	void setModelMatrix(glm::mat4 model) { ImageMapper::model = model; }

	std::string getMapperName() override { return "ImageMapper"; }

	// Updates the buffers to match the input data
	void update() override;

	void useShader(std::string shaderGroup) override;

	void draw(Renderer* ren) override;

	GLuint getShaderProgramID() override;

protected:
	void updateBuffer();

protected:
	ImageData* imageData = nullptr;
	PlaneSource* planeSource = nullptr;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 imageSizeMat = glm::mat4(1.0f);

	GLuint vboID = -1;
	GLuint vaoID = -1;
	GLuint texID = -1;

	ShaderProgram* shaderProgram = nullptr;
};