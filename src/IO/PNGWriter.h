#pragma once
#include <string>

class ImageData;

class PNGWriter
{
public:
	std::string getFileName() const { return fileName; }

	void setInput(ImageData* data) { imageData = data; }
	void setFileName(std::string fileName) { PNGWriter::fileName = fileName; }

	void update();

protected:
	ImageData* imageData = nullptr;
	std::string fileName = "";
};