#include "Geometry3D.h"
#include "PolyData.h"

namespace geom3d
{
	// Shape base
	Shape::Shape()
	{
		pos = glm::vec3(0.0f);
		type = POINT;
	}
	Shape::Shape(glm::vec3 pos)
	{
		Shape::pos = pos;
		type = POINT;
	}
	GLfloat Shape::volume() { return 0.0f; }

	// Sphere
	Sphere::Sphere()
	{
		radius = 0.0f;
		type = SPHERE;
	}
	Sphere::Sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
	{
		pos = glm::vec3(x, y, z);
		Sphere::radius = radius;
		type = SPHERE;
	}
	Sphere::Sphere(glm::vec3 center, GLfloat radius)
	{
		pos = center;
		Sphere::radius = radius;
		type = SPHERE;
	}
	GLfloat Sphere::volume() { return 4.0f * PI * radius * radius * radius / 3.0f; }

	// Rect
	Rect::Rect()
	{
		extent = glm::vec3(0.0f);
		type = RECT;
	}
	Rect::Rect(glm::vec3 center, glm::vec3 size)
	{
		pos = center;
		extent = size * 0.5f;
		type = RECT;
	}
	GLfloat Rect::volume() { return extent.x * extent.y * extent.z * 8.0f; }
	glm::vec3 Rect::size() { return extent * 2.0f; }
	glm::vec3 Rect::origin() { return pos - extent; }

	// Ray
	Ray::Ray()
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		dir = glm::vec3(1.0f, 0.0f, 0.0f);
		type = RAY;
	}
	Ray::Ray(glm::vec3 start, glm::vec3 dir)
	{
		pos = start;
		Ray::dir = dir;
		type = RAY;
	}
}