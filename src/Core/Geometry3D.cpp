#include "Geometry3D.h"

namespace geom3d
{
	// Shape base
	Shape::Shape()
	{
		pos = glm::vec3(0.0f);
		type = ShapeType::POINT;
	}
	Shape::Shape(glm::vec3 pos)
	{
		Shape::pos = pos;
		type = ShapeType::POINT;
	}
	GLfloat Shape::volume() const { return 0.0f; }

	// Sphere
	Sphere::Sphere()
	{
		radius = 0.0f;
		type = ShapeType::SPHERE;
	}
	Sphere::Sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
	{
		pos = glm::vec3(x, y, z);
		this->radius = radius;
		type = ShapeType::SPHERE;
	}
	Sphere::Sphere(glm::vec3 center, GLfloat radius)
	{
		pos = center;
		this->radius = radius;
		type = ShapeType::SPHERE;
	}
	GLfloat Sphere::volume() const { return 4.0f * PI * radius * radius * radius / 3.0f; }

	// Rect
	Rect::Rect()
	{
		extent = glm::vec3(0.0f);
		type = ShapeType::RECT;
	}
	Rect::Rect(glm::vec3 center, glm::vec3 size)
	{
		pos = center;
		extent = size * 0.5f;
		type = ShapeType::RECT;
	}
	GLfloat Rect::volume() const { return extent.x * extent.y * extent.z * 8.0f; }
	glm::vec3 Rect::size() const { return extent * 2.0f; }
	glm::vec3 Rect::origin() const { return pos - extent; }
	GLfloat* Rect::bounds() const
	{
		static GLfloat results[6];
		results[0] = pos.x - extent.x;
		results[1] = pos.x + extent.x;
		results[2] = pos.y - extent.y;
		results[3] = pos.y + extent.y;
		results[4] = pos.z - extent.z;
		results[5] = pos.z + extent.z;
		return results;
	}

	// Ray
	Ray::Ray()
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		dir = glm::vec3(1.0f, 0.0f, 0.0f);
		type = ShapeType::RAY;
	}
	Ray::Ray(glm::vec3 start, glm::vec3 dir)
	{
		pos = start;
		this->dir = dir;
		type = ShapeType::RAY;
	}
}