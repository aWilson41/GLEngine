#include "PNGReader.h"
#include "ImageData.h"
#include "LodePNG/lodepng.h"

PNGReader::PNGReader() { imageData = std::make_shared<ImageData>(); }

void PNGReader::update()
{
	if (fileName == "")
	{
		printf("PNGReader: No file name specified.\n");
		return;
	}

	imageData->clear();

	std::vector<unsigned char> png;
	std::vector<unsigned char> image;
	if (lodepng::load_file(png, fileName) != 0)
	{
		printf(("Error reading: " + fileName + "\n").c_str());
		return;
	}

	UINT width, height;
	if (lodepng::decode(image, width, height, png) != 0)
	{
		printf(("Error reading: " + fileName + "\n").c_str());
		return;
	}

	imageData = std::make_shared<ImageData>();
	UINT dim[] = { width, height, 1 };
	double spacing[] = { 1.0, 1.0, 1.0 };
	double origin[] = { 0.0, 0.0, 0.0 };
	imageData->allocate2DImage(dim, spacing, origin, 3, ScalarType::UCHAR_T);
	unsigned char* data = imageData->getData<unsigned char>();

	// Copy the loaded image
	for (UINT y1 = 0, y2 = height - 1; y1 < height; y1++, y2--)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT i1 = (x + y1 * width) * 3;
			UINT i2 = (x + y2 * width) * 4;
			data[i1] = image[i2]; // r
			data[i1 + 1] = image[i2 + 1]; // g
			data[i1 + 2] = image[i2 + 2]; // b
		}
	}
}