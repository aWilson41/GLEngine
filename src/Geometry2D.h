#pragma once
#include "MathHelper.h"

namespace geom2d
{
	enum ShapeType
	{
		POINT = 0,
		CIRCLE = 1,
		RECT = 2
	};

	class Shape
	{
	public:
		Shape();
		Shape(glm::vec2 pos);

	public:
		virtual GLfloat area() const;

	public:
		ShapeType type = ShapeType::POINT;
		glm::vec2 pos = glm::vec2(0.0f);
	};

	class Circle : public Shape
	{
	public:
		Circle();
		Circle(glm::vec2 pos, GLfloat radius);
		Circle(GLfloat x, GLfloat y, GLfloat radius);

	public:
		GLfloat area() const override;

	public:
		GLfloat radius = 0.0f;
	};

	class Rect : public Shape
	{
	public:
		Rect();
		Rect(glm::vec2 center, glm::vec2 size);

	public:
		GLfloat area() const override;
		glm::vec2 topLeft() const;
		glm::vec2 topRight() const;
		glm::vec2 bottomRight() const;
		glm::vec2 bottomLeft() const;
		glm::vec2 size() const;

	public:
		glm::vec2 extent = glm::vec2(0.0f);
	};

	class Ray
	{
	public:
		Ray(glm::vec3 start, glm::vec3 direction);

	public:
		glm::vec3 start;
		glm::vec3 direction;
	};
}