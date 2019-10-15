#pragma once
#include "MathHelper.h"

class PolyData;

// Defines a uv sphere (defaults 0.5 radius for 1x1x1 sphere)
// TODO: Other types of spheres, namely icosphere (subdivision of tetrahedron)
class SphereSource
{
public:
	SphereSource();
	~SphereSource();

public:
	PolyData* getOutput() { return outputData; }
	// Updates the output poly data to the sphere specified by the parameters
	void update();

public:
	glm::vec3 getOrigin() { return origin; }
	GLfloat getRadius() { return radius; }
	GLuint getThetaDivisions() { return divTheta; }
	GLuint getPhiDivisions() { return divPhi; }

	void setOrigin(glm::vec3 origin) { SphereSource::origin = origin; }
	void setOrigin(GLfloat x, GLfloat y, GLfloat z) { origin = glm::vec3(x, y, z); }
	void setRadius(GLfloat radius) { SphereSource::radius = radius; }
	void setDivisions(GLuint divTheta, GLuint divPhi)
	{
		SphereSource::divTheta = divTheta;
		SphereSource::divPhi = divPhi;
	}

protected:
	PolyData* outputData = nullptr;

	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat radius = 0.5f;
	GLuint divTheta = 8;
	GLuint divPhi = 8;
};