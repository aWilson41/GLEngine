#pragma once
#include "MathHelper.h"
#include "Geometry3D.h"

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
	bool getOptimizeByRadius() { return optimizeByRadius; }
	GLfloat* getBounds() { return bounds; }
	UINT getNumberOfPoints() { return numPts; }
	GLfloat getRadius() { return radius; }
	UINT getNumberOfIterations() { return numIterations; }

	// Projects polydata onto a plane to do the calculation easier, Default: false
	void setUse2d(bool use2d) { PolyDataPointCloud::use2d = use2d; }
	// Iteratively moves points away from each other for better distribution, Default: false
	void setOptimizeByRadius(bool optimizeByRadius) { PolyDataPointCloud::optimizeByRadius = optimizeByRadius; }
	// Number of points to generate, Default: 1000
	void setNumberOfPoints(UINT numPts) { PolyDataPointCloud::numPts = numPts; }
	// Radius for iterative optimization, Default: 0.05
	void setRadius(GLfloat radius) { PolyDataPointCloud::radius = radius; }
	// Radius for iterative optimization, Default: 0.05
	void setNumberOfIterations(UINT numIterations) { PolyDataPointCloud::numIterations = numIterations; }

private:
	PolyData* inputData = nullptr;
	PolyData* outputData = nullptr;

	bool use2d = false;
	bool optimizeByRadius = false;
	GLfloat bounds[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	UINT numPts = 1000;
	GLfloat radius = 0.05f;
	UINT numIterations = 20;
};