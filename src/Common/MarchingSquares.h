#pragma once
#include "MathHelper.h"
#include <memory>

class ImageData;
class PolyData;
class ShaderProgram;

// Compute polygon from implicit surface using computer shader
class MarchingSquares
{
public:
	MarchingSquares();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::shared_ptr<ImageData> getInput() const { return inputData; }

	void setInput(std::shared_ptr<ImageData> inputData) { this->inputData = inputData; }
	void setIsoValue(GLfloat isoValue) { this->isoValue = isoValue; }

	void update();

private:
	std::shared_ptr<ImageData> inputData = nullptr;
	std::shared_ptr<PolyData> outputData = nullptr;

	GLfloat isoValue = 0.0f;

	//std::shared_ptr<ShaderProgram> shaderProgram = nullptr;
};