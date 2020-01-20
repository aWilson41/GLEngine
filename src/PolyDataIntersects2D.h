#pragma once
#include <vector>
#include <memory>

class ContactManifold2D;
class PolyData;

// PolyData filter for computing 2d polygon intersections
// This an edge based intersection test it does not handle total overlap
// It also does not return resolution depth
// Resolution direction is direction to move 1
// Expects a counter clockwise edge loop
class PolyDataIntersects2D
{
public:
	PolyDataIntersects2D();
	~PolyDataIntersects2D();

public:
	PolyData* getInput() const { return inputData1; }
	std::shared_ptr<ContactManifold2D> getManifold() const { return contactManifold; }
	bool isIntersecting() const { return contactManifold != nullptr; }

	void setInput1(PolyData* inputData) { this->inputData1 = inputData; }
	void setInput2(PolyData* inputData) { this->inputData2 = inputData; }

	void update();

private:
	PolyData* inputData1 = nullptr;
	PolyData* inputData2 = nullptr;

	std::shared_ptr<ContactManifold2D> contactManifold = nullptr;
	bool ComputeConvexDepths = true;
};