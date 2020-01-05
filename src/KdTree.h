#pragma once
#include "MathHelper.h"
#include <functional>

class PolyData;

class KdTreeNode
{
public:
	KdTreeNode() { }
	KdTreeNode(UINT id, UINT dim, UINT leftId, UINT rightId, GLfloat splitPlane, UINT start, UINT end, UINT parentId)
	{
		this->id = id;
		this->dim = dim;
		this->leftId = leftId;
		this->rightId = rightId;
		this->splitplane = splitPlane;
		this->start = start;
		this->end = end;
		this->parentId = parentId;
	}

public:
	GLfloat splitplane = 1.0f;
	UINT dim = -1; // Dimension to split in
	UINT id = -1; // Location in array
	UINT parentId = -1; // Location in array
	// Children location in array
	UINT leftId = -1;
	UINT rightId = -1;
	// Start and end in the index array
	UINT start = -1;
	UINT end = -1;
};

// A contigouous in memory kdtree implementation
// Construction done by median split, cycling dimension the cut is in
class KdTree
{
public:
	KdTreeNode* getNodes() { return nodes.data(); }
	UINT* getIndices() { return indices.data(); }
	UINT getNumNodes() const { return static_cast<UINT>(nodes.size()); }
	glm::vec3 getPoint(UINT i) const { return accessorFunc(i); }

	// If on outputs points in order of the sorted indices
	void setLocalOrder(bool localOrder) { KdTree::LocalOrder = localOrder; }
	void setLeafSize(UINT leafSize) { KdTree::leafSize = leafSize; }
	// Specify to get positional data
	void setAccessor(std::function<glm::vec3(UINT i)> accessorFunc, UINT numPts)
	{
		this->accessorFunc = accessorFunc;
		this->numPts = numPts;
	}

	// Constructs the kdtree
	void update();

private:
	std::function<glm::vec3(UINT i)> accessorFunc;
	UINT numPts = -1;

	// First node is root
	std::vector<KdTreeNode> nodes;
	std::vector<UINT> indices;
	bool LocalOrder = false;
	UINT leafSize = 20;
};