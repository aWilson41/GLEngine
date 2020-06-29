//#pragma once
//#include "Geometry2D.h"
//#include <functional>
//#include <list>
//#include <tuple>
//#include <unordered_map>
//
//class QuadTreeNode
//{
//public:
//	QuadTreeNode() { }
//	QuadTreeNode(UINT id, geom2d::Rect rect)
//	{
//		this->id = id;
//		this->rect = rect;
//	}
//
//public:
//	geom2d::Rect rect;
//
//	// Id of node in nodes array
//	UINT id = -1;
//	// Id of children in nodes array
//	int childIds[4] = { -1, -1, -1, -1 };
//	// Point indices
//	std::list<UINT> indices;
//};
//
//// Quadtree for points
//// Stopping conditions used: Bin point count, bin min size
//// This is actually a multi quadtree. Top level hash of quadtrees, when points are added
//// outside of the bounds of the original quadtree we produce a new quadtree in a spatial
//// hash (equal size of the root bounds initially provided)
//class QuadTree
//{
//public:
//	float getMinEdgeSize() const { return minEdgeSize; }
//	float getMaxEdgeSize() const { return maxEdgeSize; }
//	UINT getMaxCount() const { return maxCount; }
//
//	void setAccessor(std::function<glm::vec3(const UINT& i)> accessorFunc, UINT numPts)
//	{
//		this->accessorFunc = accessorFunc;
//		this->numPts = numPts;
//	}
//	void setMinEdgeSize(float minEdgeSize) { this->minEdgeSize = minEdgeSize; }
//	void setMaxCount(UINT maxCount) { this->maxCount = maxCount; }
//
//public:
//	// Initialize the quadtree using pointer to point data
//	void init(glm::vec3* pts, UINT numPts, geom2d::Rect* initBounds = nullptr);
//
//	// Update the quadtree
//	void update();
//
//private:
//	std::function<glm::vec3(const UINT& i)> accessorFunc;
//	UINT numPts = -1;
//
//	// Set of all nodes
//	std::vector<QuadTreeNode> nodes;
//	// Handles to the root nodes
//	std::unordered_map<glm::ivec2, UINT> roots;
//	// Node assignments of all points
//	std::vector<UINT> indices;
//
//	geom2d::Rect initBounds = geom2d::Rect(glm::vec2(0.0f), glm::vec2(0.0f));
//	UINT maxCount = 8;
//	float minEdgeSize = 1.0f;
//	float maxEdgeSize = 100.0f;
//};