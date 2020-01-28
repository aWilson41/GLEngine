#pragma once
#include "MathHelper.h"
#include <memory>

class PolyData;

// Defines a quad (1x1 on XY plane, center at 0.0)
class QuadSource
{
public:
	QuadSource();

public:
	std::shared_ptr<PolyData> getOutput() const { return outputData; }
	glm::vec3 getOrigin() const { return origin; }
	glm::vec3 getP1() const { return p1; }
	glm::vec3 getP2() const { return p2; }

	void setOrigin(glm::vec3 origin) { this->origin = origin; }
	void setOrigin(GLfloat x, GLfloat y, GLfloat z) { this->origin = glm::vec3(x, y, z); }
	void setP1(glm::vec3 p1) { this->p1 = p1; }
	void setP1(GLfloat x, GLfloat y, GLfloat z) { this->p1 = glm::vec3(x, y, z); }
	void setP2(glm::vec3 p2) { this->p2 = p2; }
	void setP2(GLfloat x, GLfloat y, GLfloat z) { this->p2 = glm::vec3(x, y, z); }

	// Updates the output poly data to the plane specified by the parameters
	void update();

protected:
	std::shared_ptr<PolyData> outputData = nullptr;

	glm::vec3 origin = glm::vec3(-0.5f, -0.5f, 0.0f);
	glm::vec3 p1 = glm::vec3(0.5f, -0.5f, 0.0f);
	glm::vec3 p2 = glm::vec3(-0.5f, 0.5f, 0.0f);
};