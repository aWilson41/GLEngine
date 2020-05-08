#pragma once
#include <string>
#include <memory>

class ImageData;

class RawImageWriter
{
public:
	std::string getFileName() const { return fileName; }
	bool getCharsAsInts() const { return charsAsInts; }

	void setInput(std::shared_ptr<ImageData> imageData) { this->imageData = imageData; }
	void setFileName(std::string fileName) { this->fileName = fileName; }
	void setCharsAsInts(bool charsAsInts) { this->charsAsInts = charsAsInts; }

	void update();

protected:
	std::shared_ptr<ImageData> imageData = nullptr;
	std::string fileName = "";
	bool charsAsInts = true;
};