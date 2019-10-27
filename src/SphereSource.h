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
	PolyData* getOutput() const { return outputData; }
	glm::vec3 getOrigin() const { return origin; }
	GLfloat getRadius() const { return radius; }
	GLuint getThetaDivisions() const { return divTheta; }
	GLuint getPhiDivisions() const { return divPhi; }

	void setOrigin(glm::vec3 origin) { SphereSource::origin = origin; }
	void setOrigin(GLfloat x, GLfloat y, GLfloat z) { origin = glm::vec3(x, y, z); }
	void setRadius(GLfloat radius) { SphereSource::radius = radius; }
	void setDivisions(GLuint divTheta, GLuint divPhi)
	{
		SphereSource::divTheta = divTheta;
		SphereSource::divPhi = divPhi;
	}

	// Updates the output poly data to the sphere specified by the parameters
	void update();

protected:
	PolyData* outputData = nullptr;

	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat radius = 0.5f;
	GLuint divTheta = 8;
	GLuint divPhi = 8;
};