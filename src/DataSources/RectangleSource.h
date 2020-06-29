#pragma once
#include "MathHelper.h"
#include <memory>

class PolyData;

// Defines a 2d rectangle
class RectangleSource
{
public:
	RectangleSource();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	glm::vec3 getOrigin() const { return origin; }
	glm::vec3 getP1() const { return points[0]; }
	glm::vec3 getP2() const { return points[1]; }

	void setOrigin(glm::vec3 center) { this->origin = center; }
	void setOrigin(GLfloat x, GLfloat y, GLfloat z) { origin = glm::vec3(x, y, z); }
	void setP1(glm::vec3 p1) { points[0] = p1; }
	void setP1(GLfloat x, GLfloat y, GLfloat z) { points[0] = glm::vec3(x, y, z); }
	void setP2(glm::vec3 p2) { points[1] = p2; }
	void setP2(GLfloat x, GLfloat y, GLfloat z) { points[1] = glm::vec3(x, y, z); }
	// Axis aligned extents
	void setOriginExtent(glm::vec2 origin, glm::vec2 extent)
	{
		this->origin = glm::vec3(origin - extent, 0.0f);
		points[0] = this->origin + glm::vec3(extent.x * 2.0f, 0.0f, 0.0f);
		points[1] = this->origin + glm::vec3(0.0f, extent.y * 2.0f, 0.0f);
	}

	// Updates the output poly data to the circle specified by the parameters
	void update();

protected:
	std::shared_ptr<PolyData> outputData = nullptr;

	glm::vec3 origin = glm::vec3(-0.5f, -0.5f, 0.0f);
	glm::vec3 points[2] = { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(-0.5f, 0.5f, 0.0f) };
};