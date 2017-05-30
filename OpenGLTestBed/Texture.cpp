#include "Texture.h"

Texture::Texture()
{

}

Texture::Texture(std::string name, UINT width, UINT height, TextureType type)
{
	Texture::type = type;
	Texture::name = name;
	SetDim(width, height);
}

Texture::Texture(std::string name, UINT width, UINT height, unsigned char* data, TextureType type)
{
	Texture::type = type;
	Texture::name = name;
	SetDim(width, height);
	buffer = data;
}


void Texture::SetPixel(int x, int y, Color color)
{
	if (x >= 0 && y >= 0 && x < (int)width && y < (int)height)
	{
		int index = Utility::Index2DTo1D(x, y, width) * 3;
		buffer[index] = color.r;
		buffer[index + 1] = color.g;
		buffer[index + 2] = color.b;
	}
}

Color Texture::GetPixel(int x, int y)
{
	if (x >= 0 && y >= 0 && x < (int)width && y < (int)height)
	{
		int index = Utility::Index2DTo1D(x, y, width) * 3;
		return Color(buffer[index], buffer[index + 1], buffer[index + 2]);
	}

	return Color(0, 0, 0);
}

Color Texture::GetPixel(UINT i)
{
	if (i >= 0 && i < width * height)
		return Color(buffer[i], buffer[i + 1], buffer[i + 2]);
	else
		return Color(0, 0, 0);
}

void Texture::SetDim(UINT width, UINT height)
{
	Texture::width = width;
	Texture::height = height;
}

void Texture::Fill(unsigned char grayScaleColor)
{
	if (buffer)
		delete[] buffer;

	// 3 rgb channels so we multiply 3
	buffer = new unsigned char[width * height * 3];
	std::fill_n(buffer, width * height * 3, 0);
}

void Texture::SetBuffer(UINT width, UINT height, unsigned char* buffer)
{
	if (Texture::buffer != nullptr)
		delete[] Texture::buffer;

	SetDim(width, height);
	Texture::buffer = buffer;
}