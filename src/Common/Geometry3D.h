#pragma once
#include "MathHelper.h"

class PolyData;

// Just a little 3d geometry helper
// Serves as a lightweight abstraction for geometric functions
namespace geom3d
{
	enum class ShapeType
	{
		POINT = 0,
		SPHERE = 1,
		RECT = 2,
		RAY = 3,
		ORIENTEDRECT = 4
	};

	class Shape
	{
	public:
		Shape();
		Shape(glm::vec3 pos);

	public:
		virtual GLfloat volume() const;

	public:
		ShapeType type = ShapeType::POINT;
		glm::vec3 pos = glm::vec3(0.0f);
	};

	class Sphere : public Shape
	{
	public:
		Sphere();
		Sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
		Sphere(glm::vec3 center, GLfloat radius);

	public:
		GLfloat volume() const override;

	public:
		GLfloat radius = 0.0f;
	};

	class Rect : public Shape
	{
	public:
		Rect();
		Rect(glm::vec3 center, glm::vec3 size);

	public:
		GLfloat volume() const override;
		glm::vec3 size() const;
		glm::vec3 origin() const;
		GLfloat* bounds() const;

	public:
		glm::vec3 extent = glm::vec3(0.0f);
	};

	class OrientedRect : public Rect
	{
	public:
		OrientedRect()
		{
			orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			type = ShapeType::ORIENTEDRECT;
		}
		OrientedRect(glm::vec3 center, glm::vec3 size, glm::quat orientation) : Rect(center, size)
		{
			this->orientation = orientation;
			type = ShapeType::ORIENTEDRECT;
		}

	public:
		glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	};

	class Ray : public Shape
	{
	public:
		Ray();
		Ray(glm::vec3 start, glm::vec3 dir);

	public:
		glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f);
	};

	// Returns first intersection
	static bool intersectSphereRay(Sphere sphere, Ray ray)
	{
		glm::vec3 m = ray.pos - sphere.pos;
		GLfloat b = glm::dot(m, ray.dir);
		GLfloat c = glm::dot(m, m) - sphere.radius * sphere.radius;

		// Exit if r�s origin outside s (c > 0) and r pointing away from s (b > 0) 
		if (c > 0.0f && b > 0.0f)
			return false;
		GLfloat discr = b * b - c;

		// A negative discriminant corresponds to ray missing sphere 
		if (discr < 0.0f)
			return false;

		// Ray now found to intersect sphere, compute smallest t value of intersection
		GLfloat t = -b - std::sqrt(discr);

		// If t is negative, ray started inside sphere so clamp t to zero 
		if (t < 0.0f)
			t = 0.0f;
		glm::vec3 q = ray.pos + t * ray.dir;

		return true;
	}
}