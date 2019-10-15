#pragma once
#include "MathHelper.h"

class PolyData;

// Defines a 2d circle of div line segments on xy plane
class CircleSource
{
public:
	CircleSource();
	~CircleSource();

public:
	PolyData* getOutput() { return outputData; }
	// Updates the output poly data to the circle specified by the parameters
	void update();

public:
	glm::vec3 getOrigin() { return origin; }
	GLfloat getRadius() { return radius; }
	GLuint getDivisions() { return div; }

	void setOrigin(glm::vec3 origin) { CircleSource::origin = origin; }
	void setOrigin(GLfloat x, GLfloat y, GLfloat z = 0.0f) { origin = glm::vec3(x, y, z); }
	void setRadius(GLfloat radius) { CircleSource::radius = radius; }
	void setDivisions(GLuint div) { CircleSource::div = div; }

protected:
	PolyData* outputData = nullptr;

	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat radius = 0.5f;
	GLuint div = 15;
};