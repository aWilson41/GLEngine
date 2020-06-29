#pragma once
#include "MathHelper.h"
#include <memory>

class ImageData;

// Resample an image to different dimensions
class ImageDataResample
{
public:
	ImageDataResample();

public:
	std::shared_ptr<ImageData> getOutput() const { return outputData; }
	std::shared_ptr<ImageData> getInput() const { return inputData; }
	glm::uivec3 getDimensions() const { return dim; }

	void setInput(std::shared_ptr<ImageData> inputData) { this->inputData = inputData; }
	void setDimensions(const glm::uivec3& dim) { this->dim = dim; }
	void setDimensions(UINT width, UINT height, UINT depth = 1) { this->dim = glm::uivec3(width, height, depth); }

	void update();

private:
	std::shared_ptr<ImageData> inputData = nullptr;
	std::shared_ptr<ImageData> outputData = nullptr;
	glm::uivec3 dim = glm::uivec3(0);
};