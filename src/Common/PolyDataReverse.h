#pragma once
#include <memory>

class PolyData;

// Reverse the winding of a polygon
class PolyDataReverse
{
public:
	PolyDataReverse();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::shared_ptr<PolyData> getInput() const { return inputData; }

	void setInput(std::shared_ptr<PolyData> inputData) { this->inputData = inputData; }

	void update();

private:
	std::shared_ptr<PolyData> inputData = nullptr;
	std::shared_ptr<PolyData> outputData = nullptr;
};