#pragma once
#include "MathHelper.h"

class PolyData;

class KdTreeNode
{
public:
	KdTreeNode() { }
	KdTreeNode(GLuint id, GLuint dim, GLuint leftId, GLuint rightId, GLfloat splitPlane)
	{
		this->id = id;
		this->dim = dim;
		this->leftId = leftId;
		this->rightId = rightId;
		this->splitplane = splitPlane;
	}

public:
	GLfloat splitplane = 1.0f;
	GLuint dim = -1; // Dimension to split in
	GLuint id = -1; // Location in array
	// Children location in array
	GLuint leftId = -1;
	GLuint rightId = -1;
};

// Construction done by median split, cycling dimension the cut is in
class KdTree
{
public:
	// If on outputs vtkPolyData with reordered points that are locally arranged
	void setLocalOrder(bool localOrder) { KdTree::LocalOrder = localOrder; }
	void setLeafSize(UINT leafSize) { KdTree::leafSize = leafSize; }
	void setInput(PolyData* inputData) { KdTree::inputData = inputData; }

	// Constructs the kdtree
	void update();

private:
	PolyData* inputData = nullptr;
	// First node is root
	std::vector<KdTreeNode> nodes;
	bool LocalOrder = false;
	UINT leafSize = 20;
};