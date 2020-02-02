#pragma once
#include <memory>

class PolyData;

// Generates 2d or 3d point clouds from polydata input
class AppendPolyData
{
public:
	AppendPolyData();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::shared_ptr<PolyData> getInput1() const { return inputData1; }
	std::shared_ptr<PolyData> getInput2() const { return inputData2; }

	void setInput1(std::shared_ptr<PolyData> inputData1) { this->inputData1 = inputData1; }
	void setInput2(std::shared_ptr<PolyData> inputData2) { this->inputData2 = inputData2; }

	void update();

private:
	std::shared_ptr<PolyData> inputData1 = nullptr;
	std::shared_ptr<PolyData> inputData2 = nullptr;
	std::shared_ptr<PolyData> outputData = nullptr;
};