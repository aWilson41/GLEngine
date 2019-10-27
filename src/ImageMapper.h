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
	ImageData* getInput() const { return imageData; }
	glm::mat4 getModelMatrix() const { return model; }
	GLuint getShaderProgramID() const override;
	std::string getMapperName() const override { return "ImageMapper"; }

	void setInput(ImageData* data);
	void setModelMatrix(glm::mat4 model) { ImageMapper::model = model; }

	// Updates the buffers to match the input data
	void update() override;

	void useShader(std::string shaderGroup) override;
	void draw(Renderer* ren) const override;

protected:
	void updateBuffer();

protected:
	ImageData* imageData = nullptr;
	PlaneSource* planeSource = nullptr;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 imageSizeMat = glm::mat4(1.0f);

	GLuint vboID = -1;
	GLuint iboID = -1;
	GLuint vaoID = -1;
	GLuint texID = -1;

	ShaderProgram* shaderProgram = nullptr;
};