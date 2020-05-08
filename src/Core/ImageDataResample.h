#pragma once
#include "MathHelper.h"
#include <memory>

class ImageData;

// Resample a 2d image to different dimensions
class ImageDataResample
{
public:
	ImageDataResample();

public:
	std::shared_ptr<ImageData> getOutput() const { return outputData; }
	std::shared_ptr<ImageData> getInput() const { return inputData; }
	glm::uivec2 getDimensions() const { return dim; }

	void setInput(std::shared_ptr<ImageData> inputData) { this->inputData = inputData; }
	void setDimensions(const glm::uivec2& dim) { this->dim = dim; }
	void setDimensions(UINT width, UINT height) { this->dim = glm::uivec2(width, height); }

	void update();

private:
	std::shared_ptr<ImageData> inputData = nullptr;
	std::shared_ptr<ImageData> outputData = nullptr;
	glm::uivec2 dim = glm::uivec2(0);
};