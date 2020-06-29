#pragma once
#include <memory>

class PolyData;

enum class TriangulateType
{
	EARCLIP = 0,
	MONOTONE = 1,
	DELAUNAY = 2
};

// PolyData filter for triangulating 2d polygons
// Can do ear clipping or monotone trinagulation. Delaunay coming soon.
// Assumes manifold counter clockwise input
class PolyDataTriangulate
{
public:
	PolyDataTriangulate();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::shared_ptr<PolyData> getInput() const { return inputData; }
	TriangulateType getType() const { return type; }

	void setInput(std::shared_ptr<PolyData> inputData) { this->inputData = inputData; }
	void setType(TriangulateType type) { this->type = type; }

	void update();

private:
	std::shared_ptr<PolyData> inputData = nullptr;
	std::shared_ptr<PolyData> outputData = nullptr;
	TriangulateType type = TriangulateType::EARCLIP;
};