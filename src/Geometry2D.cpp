#include "Geometry2D.h"

namespace geom2d
{
	// Shape Base
	Shape::Shape()
	{
		pos = glm::vec2(0.0f);
		type = POINT;
	}
	Shape::Shape(glm::vec2 pos)
	{
		Shape::pos = pos;
		type = POINT;
	}
	GLfloat Shape::area() { return 0.0f; }

	// Circle
	Circle::Circle()
	{
		radius = 0.0f;
		type = CIRCLE;
	}
	Circle::Circle(glm::vec2 pos, GLfloat radius)
	{
		Circle::pos = pos;
		Circle::radius = radius;
		type = CIRCLE;
	}
	Circle::Circle(GLfloat x, GLfloat y, GLfloat radius)
	{
		Circle::pos = glm::vec2(x, y);
		Circle::radius = radius;
		type = CIRCLE;
	}
	GLfloat Circle::area() { return PI * radius * radius; }

	// Rectangle
	Rect::Rect()
	{
		extent = glm::vec2(0.0f);
		type = RECT;
	}
	Rect::Rect(glm::vec2 center, glm::vec2 size)
	{
		pos = center;
		extent = size * 0.5f;
		type = RECT;
	}
	GLfloat Rect::area() { return extent.x * extent.y * 4.0f; }
	glm::vec2 Rect::topLeft() { return pos - extent; }
	glm::vec2 Rect::topRight() { return pos + glm::vec2(extent.x, -extent.y); }
	glm::vec2 Rect::bottomRight() { return pos + extent; }
	glm::vec2 Rect::bottomLeft() { return pos + glm::vec2(-extent.x, extent.y); }
	glm::vec2 Rect::size() { return extent * 2.0f; }

	// Ray
	Ray::Ray(glm::vec3 start, glm::vec3 direction)
	{
		Ray::start = start;
		Ray::direction = direction;
	}
}