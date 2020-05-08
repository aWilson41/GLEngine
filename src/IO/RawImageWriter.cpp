#include "RawImageWriter.h"
#include "ImageData.h"
#include <fstream>
#include <unordered_map>

// Maps cell type to mode
std::unordered_map<ScalarType, UINT> typeMap = {
	{ ScalarType::UCHAR_T, 0 },
	{ ScalarType::INT_T, 1 },
	{ ScalarType::FLOAT_T, 2 },
	{ ScalarType::DOUBLE_T, 3 } };

template<class T>
static void writeImage(std::shared_ptr<ImageData> imageData, std::string fileName, T, bool charsAsInts)
{
	T* imagePtr = static_cast<T*>(imageData->getData());
	UINT* dim = imageData->getDimensions();
	UINT numComps = imageData->getNumComps();

	std::ofstream file;
	file.open(fileName);

	file << dim[0] << ' ' << dim[1] << ' ' << dim[2] << ' ' << numComps << ' ' << typeMap[imageData->getScalarType()] << '\n';
	if (std::is_same<T, unsigned char>::value && charsAsInts)
	{
		for (UINT i = 0; i < dim[0] * dim[1] * dim[2] * numComps; i++)
		{
			file << static_cast<int>(imagePtr[i]) << ' ';
		}
	}
	else
	{
		for (UINT i = 0; i < dim[0] * dim[1] * dim[2] * numComps; i++)
		{
			file << imagePtr[i] << ' ';
		}
	}
	file.close();
}

void RawImageWriter::update()
{
	if (fileName == "")
	{
		printf("RawImageWriter: No file name specified.\n");
		return;
	}

	switch (imageData->getScalarType())
	{
		TemplateMacro(writeImage(imageData, fileName, static_cast<TT>(0), charsAsInts));
	default:
		printf("RawImageWriter: Not supported image type.\n");
		break;
	}
}