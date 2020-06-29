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
	GLfloat Shape::area() const { return 0.0f; }

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
	GLfloat Circle::area() const { return PI * radius * radius; }

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
	GLfloat Rect::area() const { return extent.x * extent.y * 4.0f; }
	GLfloat* Rect::getBounds()
	{
		static GLfloat bounds[4] = { pos.x - extent.x, pos.x + extent.x, pos.y - extent.y, pos.y + extent.y };
		return bounds;
	}
	glm::vec2 Rect::topLeft() const { return pos - extent; }
	glm::vec2 Rect::topRight() const { return pos + glm::vec2(extent.x, -extent.y); }
	glm::vec2 Rect::bottomRight() const { return pos + extent; }
	glm::vec2 Rect::bottomLeft() const { return pos + glm::vec2(-extent.x, extent.y); }
	glm::vec2 Rect::size() const { return extent * 2.0f; }
	float Rect::left() const { return pos.x - extent.x; }
	float Rect::right() const { return pos.x + extent.x; }
	float Rect::top() const { return pos.y + extent.y; }
	float Rect::bottom() const { return pos.y - extent.y; }

	// Ray
	Ray::Ray(glm::vec3 start, glm::vec3 direction)
	{
		Ray::start = start;
		Ray::direction = direction;
	}
}