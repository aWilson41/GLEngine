#pragma once
#include "MathHelper.h"
#include "Types.h"

// Stores image data of uchar, int, float, or double
class ImageData
{
public:
	ImageData() = default;
	~ImageData() { clear(); }

public:
	UINT* getDimensions() { return dim; }
	double* getBounds() { return bounds; }
	double* getSpacing() { return spacing; }
	double* getOrigin() { return origin; }
	template<typename T>
	T* getData() { return static_cast<T*>(data); }
	void* getData() { return data; }
	UINT getNumComps() const { return numComps; }
	ScalarType getScalarType() const { return type; }
	template<typename T>
	T* getScalarRange()
	{
		T* data = getData<T>();
		static T range[2] = { static_cast<T>(FLOAT_MAX), static_cast<T>(FLOAT_MIN) };
		for (UINT i = 0; i < dim[0] * dim[1] * dim[2]; i++)
		{
			range[0] = std::min(data[i], range[0]);
			range[1] = std::max(data[i], range[1]);
		}
		return range;
	}

	void setSpacing(double x, double y, double z)
	{
		spacing[0] = x;
		spacing[1] = y;
		spacing[2] = z;
	}
	void setOrigin(double x, const double y, double z)
	{
		origin[0] = x;
		origin[1] = y;
		origin[2] = z;
	}

	void allocate2DImage(UINT* dim, double* spacing, double* origin, UINT numComps, ScalarType type);
	void allocate3DImage(UINT* dim, double* spacing, double* origin, UINT numComps, ScalarType type);

	// Updates bounds given dimensions, spacing, & origin
	void updateBounds();

	void clear();

protected:
	template<typename T>
	void clearData(const UINT count)
	{
		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	template<typename T>
	void allocateData(const UINT count)
	{
		if (data != nullptr)
			delete[] data;
		data = new T[count];
	}

protected:
	void* data = nullptr;
	UINT dim[3] = { 0, 0, 0 };
	UINT numComps = 1;
	ScalarType type = ScalarType::UCHAR_T;

	double spacing[3] = { 1.0, 1.0, 1.0 };
	double origin[3] = { 0.0, 0.0, 0.0 };
	double bounds[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
};