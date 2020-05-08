#pragma once
#include <memory>
#include <string>

class ImageData;

class PNGReader
{
public:
	PNGReader();

public:
	std::shared_ptr<ImageData> getOutput() const { return imageData; }
	std::string getFileName() const { return fileName; }

	void setFileName(std::string fileName) { PNGReader::fileName = fileName; }

	void update();

protected:
	std::shared_ptr<ImageData> imageData = nullptr;
	std::string fileName = "";
};