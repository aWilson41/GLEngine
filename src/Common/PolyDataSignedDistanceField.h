#pragma once
#include "MathHelper.h"
#include <memory>

class ImageData;
class PolyData;

// Computes ImageData signed distance field from polygon
class PolyDataSignedDistanceField
{
public:
	PolyDataSignedDistanceField();

public:
	std::shared_ptr<ImageData> getOutput() const { return outputData; }
	std::shared_ptr<PolyData> getInput() const { return inputData; }
	double getDistanceThreshold() const { return distanceThreshold; }
	bool getUseDistanceThreshold() const { return useDistanceThreshold; }
	std::shared_ptr<ImageData> getReferenceImage() const { return referenceImage; }
	void getOutputImageInfo(UINT* dim, double* spacing, double* origin)
	{
		dim[0] = this->dim[0];
		dim[1] = this->dim[1];
		dim[2] = this->dim[2];
		spacing[0] = this->spacing[0];
		spacing[1] = this->spacing[1];
		spacing[2] = this->spacing[2];
		origin[0] = this->origin[0];
		origin[1] = this->origin[1];
		origin[2] = this->origin[2];
	}

	void setInput(std::shared_ptr<PolyData> inputData) { this->inputData = inputData; }
	// How far to compute the field from the surface
	void setDistanceThreshold(const double distanceThreshold) { this->distanceThreshold = distanceThreshold; }
	void setUseDistanceThreshold(const bool useDistanceThreshold) { this->useDistanceThreshold = useDistanceThreshold; }
	void setOutputImageInfo(UINT* dim, double* spacing, double* origin)
	{
		this->dim[0] = dim[0];
		this->dim[1] = dim[1];
		this->dim[2] = dim[2];
		this->spacing[0] = spacing[0];
		this->spacing[1] = spacing[1];
		this->spacing[2] = spacing[2];
		this->origin[0] = origin[0];
		this->origin[1] = origin[1];
		this->origin[2] = origin[2];
		useReferenceImage = false;
	}
	void setReferenceImage(std::shared_ptr<ImageData> referenceImage)
	{
		this->referenceImage = referenceImage;
		useReferenceImage = true;
	}

	void update();

private:
	std::shared_ptr<PolyData> inputData = nullptr;
	std::shared_ptr<ImageData> outputData = nullptr;

	double distanceThreshold = 1.0;
	bool useDistanceThreshold = false;

	UINT dim[3] = { 100, 100, 100 };
	double spacing[3] = { 1.0, 1.0, 1.0 };
	double origin[3] = { 0.0, 0.0, 0.0 };
	bool useReferenceImage = false;
	std::shared_ptr<ImageData> referenceImage = nullptr;
};