#pragma once
#include "MathHelper.h"

class Contact2D
{
public:
	glm::vec2 pt = glm::vec2(0.0f);
	glm::vec2 n = glm::vec2(1.0f, 0.0f);
	// Penetration depth
	GLfloat depth = 0.0f;

	// Segment indices of the two intersection line segments
	UINT segmentIndex[2] = { static_cast<UINT>(-1), static_cast<UINT>(-1) };
};

class ContactManifold2D
{
public:
	Contact2D contacts[2];
	UINT numberOfContacts = 0;
};