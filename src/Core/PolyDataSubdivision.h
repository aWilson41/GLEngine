#pragma once
#include "MathHelper.h"
#include <memory>

class PolyData;

enum class SubdivisionType
{
	LINEAR
};

// Subdivides 2d polygons
class PolyDataSubdivision
{
public:
	PolyDataSubdivision();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::shared_ptr<PolyData> getInput() const { return inputData; }
	SubdivisionType getType() const { return type; }
	UINT getSubdivisionCount() const { return subdivCount; }

	void setInput(std::shared_ptr<PolyData> inputData) { this->inputData = inputData; }
	void setType(SubdivisionType type) { this->type = type; }
	void setSubdivisionCount(UINT subdivCount) { this->subdivCount = subdivCount; }

	void update();

private:
	std::shared_ptr<PolyData> inputData = nullptr;
	std::shared_ptr<PolyData> outputData = nullptr;
	SubdivisionType type = SubdivisionType::LINEAR;
	UINT subdivCount = 1;
};