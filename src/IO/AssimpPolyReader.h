#pragma once
#include <memory>
#include <string>

class PolyData;

class AssimpPolyReader
{
public:
	AssimpPolyReader();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::string getFileName() const { return fileName; }

	void setFileName(std::string fileName) { AssimpPolyReader::fileName = fileName; }

	void update();

protected:
	std::shared_ptr<PolyData> outputData = nullptr;
	std::string fileName = "";
};