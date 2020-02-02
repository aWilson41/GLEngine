#pragma once
#include "MathHelper.h"
#include <memory>

class PolyData;

class Contact2D
{
public:
	Contact2D() { }
	Contact2D(glm::vec2 pt, glm::vec2 n)
	{
		this->pt = pt;
		this->n = n;
	}

public:
	glm::vec2 pt = glm::vec2(0.0f);
	glm::vec2 n = glm::vec2(1.0f, 0.0f);
	// Penetration depth
	GLfloat depth = 0.0f;

	// Segment indices of the two intersection line segments
	UINT segmentIndex[2] = { static_cast<UINT>(-1), static_cast<UINT>(-1) };
};

using ContactList = std::vector<std::shared_ptr<Contact2D>>;

// PolyData filter for computing 2d polygon intersections
// This is an edge based intersection test, it does not handle total overlap
// Resolution direction is direction to move 1 to leave 2
// Expects a counter clockwise edge loop
class PolyDataIntersects2D
{
public:
	std::shared_ptr<PolyData> getInput() const { return inputData1; }
	std::shared_ptr<ContactList> getContacts() { return contacts; }
	// Get the minimum depth contact
	std::shared_ptr<Contact2D> getMinContact() const;
	bool isIntersecting() const { return contacts != nullptr; }

	void setInput1(std::shared_ptr<PolyData> inputData) { this->inputData1 = inputData; }
	void setInput2(std::shared_ptr<PolyData> inputData) { this->inputData2 = inputData; }

	void update();

private:
	std::shared_ptr<PolyData> inputData1 = nullptr;
	std::shared_ptr<PolyData> inputData2 = nullptr;

	std::shared_ptr<ContactList> contacts = nullptr;
};