#pragma once
#include <vector>
#include <memory>

class ContactManifold2D;
class PolyData;

// PolyData filter for computing 2d polygon intersections
// This is an edge based intersection test, it does not handle total overlap
// Resolution direction is direction to move 1 to leave 2
// Expects a counter clockwise edge loop
class PolyDataIntersects2D
{
public:
	std::shared_ptr<PolyData> getInput() const { return inputData1; }
	std::shared_ptr<ContactManifold2D> getManifold() const { return contactManifold; }
	bool isIntersecting() const { return contactManifold != nullptr; }

	void setInput1(std::shared_ptr<PolyData> inputData) { this->inputData1 = inputData; }
	void setInput2(std::shared_ptr<PolyData> inputData) { this->inputData2 = inputData; }

	void update();

private:
	std::shared_ptr<PolyData> inputData1 = nullptr;
	std::shared_ptr<PolyData> inputData2 = nullptr;

	std::shared_ptr<ContactManifold2D> contactManifold = nullptr;
};