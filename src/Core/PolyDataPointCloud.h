#pragma once
#include "MathHelper.h"
#include "Geometry3D.h"
#include <memory>

class PolyData;

// Generates 2d or 3d point clouds from polydata input
class PolyDataPointCloud
{
public:
	PolyDataPointCloud();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	std::shared_ptr<PolyData> getInput() const { return inputData; }
	bool getUse2d() const { return use2d; }
	bool getOptimizeByRadius() const { return optimizeByRadius; }
	GLfloat* getBounds() { return bounds; }
	UINT getNumberOfPoints() const { return numPts; }
	GLfloat getRadius() const { return radius; }
	UINT getNumberOfIterations() const { return numIterations; }

	void setInput(std::shared_ptr<PolyData> inputData) { this->inputData = inputData; }
	// Projects polydata onto a plane to do the calculation easier, Default: false
	void setUse2d(bool use2d) { this->use2d = use2d; }
	// Iteratively moves points away from each other for better distribution, Default: false
	void setOptimizeByRadius(bool optimizeByRadius) { this->optimizeByRadius = optimizeByRadius; }
	// Number of points to generate, Default: 1000
	void setNumberOfPoints(UINT numPts) { this->numPts = numPts; }
	// Radius for iterative optimization, Default: 0.05
	void setRadius(GLfloat radius) { this->radius = radius; }
	// Radius for iterative optimization, Default: 0.05
	void setNumberOfIterations(UINT numIterations) { this->numIterations = numIterations; }
	// Multiplied by the step made to separate two spheres during optimization
	void setStepRatio(GLfloat stepRatio) { this->stepRatio = stepRatio; }

	void update();

private:
	std::shared_ptr<PolyData> inputData = nullptr;
	std::shared_ptr<PolyData> outputData = nullptr;

	bool use2d = false;
	bool optimizeByRadius = false;
	GLfloat bounds[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	UINT numPts = 1000;
	GLfloat radius = 0.05f;
	UINT numIterations = 20;
	GLfloat stepRatio = 0.2f;
};