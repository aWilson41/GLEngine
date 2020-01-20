#pragma once
#include <string>
#include <memory>

class ImageData;

class RawImageWriter
{
public:
	std::string getFileName() const { return fileName; }

	void setInput(std::shared_ptr<ImageData> data) { imageData = data; }
	void setFileName(std::string fileName) { RawImageWriter::fileName = fileName; }

	void update();

protected:
	std::shared_ptr<ImageData> imageData = nullptr;
	std::string fileName = "";
};