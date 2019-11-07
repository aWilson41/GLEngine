#pragma once
#include <KdTree.h>
#include <stack>

// Computes kNN for points
class KdTreePointLocator
{
public:
	void setKdTree(KdTree* kdtree) { this->kdtree = kdtree; }

	// Get N nearest points
	std::vector<UINT> getPoints(glm::vec3 pt, UINT n)
	{
		std::vector<UINT> nearestIds(n);
		//std::fill_n(nearestIds.begin(), nearestIds.size(), -1);

		//KdTreeNode* nodes = kdtree->getNodes();
		//const UINT numNodes = kdtree->getNumNodes();
		//UINT* indices = kdtree->getIndices();

		//GLfloat nearestRadiusSquared = std::numeric_limits<float>::max();
		//std::stack<KdTreeNode*> path;
		//path.push(&nodes[0]);
		//while (true)
		//{
		//	KdTreeNode* node = path.top();
		//	UINT depth = path.size();

		//	// Leaf
		//	if (node->leftId == -1 && node->rightId == -1)
		//	{
		//		for (UINT i = node->start; i < node->end; i++)
		//		{
		//			glm::vec3 diff = kdtree->getPoint(i) - pt;
		//			if (nearestRadiusSquared < glm::dot(diff, diff))
		//			{
		//				nearestIds.push_back(indices[i]);
		//				if (nearestIds.size() == n)
		//					
		//			}
		//		}
		//	}
		//}
		return nearestIds;
	}

	// Get points within radius (inclusive)
	std::vector<UINT> getPoints(glm::vec3 pt, GLfloat r)
	{
		const GLfloat sqrDiameter = 4.0f * r * r;
		std::vector<UINT> ptIds;

		KdTreeNode* nodes = kdtree->getNodes();
		const UINT numNodes = kdtree->getNumNodes();
		UINT* indices = kdtree->getIndices();

		std::vector<bool> visited(numNodes);
		std::fill_n(visited.begin(), visited.size(), false);
		std::stack<UINT> path;
		path.push(0);
		visited[0] = true;
		while (!path.empty())
		{
			KdTreeNode node = nodes[path.top()];

			// If no children, this is a leaf
			if (node.leftId == -1 && node.rightId == -1)
			{
				// Test all points of this node
				for (UINT i = node.start; i < node.end; i++)
				{
					glm::vec3 diff = kdtree->getPoint(indices[i]) - pt;
					if (glm::dot(diff, diff) <= sqrDiameter)
						ptIds.push_back(indices[i]);
				}
				// Mark node as finished
				visited[node.id] = true;
				// Remove it
				path.pop();
			}
			else
			{
				// Remove internal node
				path.pop();

				// If node exists, hasn't been visited, and is within reach of the sphere add it to be processed
				if (node.leftId != -1 && visited[node.leftId] == false && pt[node.dim] - r < node.splitplane)
					path.push(node.leftId);
				if (node.rightId != -1 && visited[node.rightId] == false && pt[node.dim] + r > node.splitplane)
					path.push(node.rightId);

				// Mark this node as finished
				visited[node.id] == true;
			}
		}
		return ptIds;
	}

public:
	KdTree* kdtree = nullptr;
};