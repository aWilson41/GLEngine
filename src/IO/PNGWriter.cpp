#include "PNGWriter.h"
#include "ImageData.h"
#include "LodePNG/lodepng.h"

void PNGWriter::update()
{
	if (fileName == "")
	{
		printf("PNGWriter: No file name specified.\n");
		return;
	}

	if (imageData->getScalarType() != ScalarType::UCHAR_T)
	{
		printf("PNGWriter: Can only write unsigned char images.\n");
		return;
	}

	// Copy the image to std vector because that's how lodepng wants it (add component)
	const UINT* dim = imageData->getDimensions();
	std::vector<unsigned char> image(dim[0] * dim[1] * 4);
	unsigned char* data = imageData->getData<unsigned char>();

	const UINT numComps = imageData->getNumComps();
	if (numComps == 1)
	{
		for (UINT y1 = 0, y2 = dim[1] - 1; y1 < dim[1]; y1++, y2--)
		{
			for (UINT x = 0; x < dim[0]; x++)
			{
				UINT i1 = x + y1 * dim[0];
				UINT i2 = (x + y2 * dim[0]) * 4;
				image[i2] = data[i1];
				image[i2 + 1] = data[i1];
				image[i2 + 2] = data[i1];
				image[i2 + 3] = 255;
			}
		}
	}
	else if (numComps == 3)
	{
		for (UINT y1 = 0, y2 = dim[1] - 1; y1 < dim[1]; y1++, y2--)
		{
			for (UINT x = 0; x < dim[0]; x++)
			{
				UINT i1 = (x + y1 * dim[0]) * 3;
				UINT i2 = (x + y2 * dim[0]) * 4;
				image[i2] = data[i1];
				image[i2 + 1] = data[i1 + 1];
				image[i2 + 2] = data[i1 + 2];
				image[i2 + 3] = 255;
			}
		}
	}
	else if (numComps == 4)
	{
		for (UINT y1 = 0, y2 = dim[1] - 1; y1 < dim[1]; y1++, y2--)
		{
			for (UINT x = 0; x < dim[0]; x++)
			{
				UINT i1 = (x + y1 * dim[0]) * 4;
				UINT i2 = (x + y2 * dim[0]) * 4;
				image[i2] = data[i1];
			}
		}
	}
	else
	{
		printf("PNGWriter: Can only write 1, 3, or 4 component images.\n");
		return;
	}

	// Write the image with lodepng
	if (lodepng::encode(fileName, image, dim[0], dim[1]))
		printf("PNGWriter: LodePNG Error\n");
}