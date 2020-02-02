#pragma once
#include <string>
#include <memory>

class ImageData;

class RawImageReader
{
public:
	std::shared_ptr<ImageData> getOutput() const { return imageData; }
	std::string getFileName() const { return fileName; }

	void setFileName(std::string fileName) { RawImageReader::fileName = fileName; }

	void update();

protected:
	std::shared_ptr<ImageData> imageData = nullptr;
	std::string fileName = "";
};