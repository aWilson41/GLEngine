#pragma once
#include <memory>
#include <string>

class ImageData;

class PNGWriter
{
public:
	std::string getFileName() const { return fileName; }

	void setInput(std::shared_ptr<ImageData> imageData) { this->imageData = imageData; }
	void setFileName(std::string fileName) { this->fileName = fileName; }

	void update();

protected:
	std::shared_ptr<ImageData> imageData = nullptr;
	std::string fileName = "";
};