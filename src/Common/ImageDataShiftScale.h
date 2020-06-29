#pragma once
#include "Types.h"
#include <memory>

class ImageData;

// Shift then scale image data
class ImageDataShiftScale
{
public:
	ImageDataShiftScale();

public:
	std::shared_ptr<ImageData> getOutput() const { return outputData; }
	std::shared_ptr<ImageData> getInput() const { return inputData; }
	float getShift() const { return shift; }
	float getScale() const { return scale; }

	void setInput(std::shared_ptr<ImageData> inputData) { this->inputData = inputData; }
	void setShift(float shift) { this->shift = shift; }
	void setScale(float scale) { this->scale = scale; }
	void setOutputType(ScalarType outputType)
	{
		this->outputType = outputType;
		useOutputType = true;
	}

	void update();

private:
	std::shared_ptr<ImageData> inputData = nullptr;
	std::shared_ptr<ImageData> outputData = nullptr;

	float shift = 0.0f;
	float scale = 1.0f;
	bool useOutputType = false;
	ScalarType outputType = ScalarType::FLOAT_T;
};