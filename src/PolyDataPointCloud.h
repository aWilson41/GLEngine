#pragma once
#include "MathHelper.h"

class PolyData;

// Generates 2d or 3d point clouds from polydata input
class PolyDataPointCloud
{
public:
	PolyDataPointCloud();
	~PolyDataPointCloud();

public:
	PolyData* getOutput() { return outputData; }
	void setInput(PolyData* inputData) { PolyDataPointCloud::inputData = inputData; }
	void update();

public:
	bool getUse2d() { return use2d; }
	UINT getNumberOfPoints() { return numPts; }

	// Default: false
	// Use 2d will project polydata onto a plane to do the calculation easier
	void setUse2d(bool use2d) { PolyDataPointCloud::use2d = use2d; }
	// Default: 1000
	// Number of points to generate
	void setNumberOfPoints(UINT numPts) { PolyDataPointCloud::numPts = numPts; }

private:
	PolyData* inputData = nullptr;
	PolyData* outputData = nullptr;

	bool use2d = false;
	UINT numPts = 1000;
};