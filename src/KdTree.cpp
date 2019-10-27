#include "KdTree.h"
#include "PolyData.h"
#include <stack>
#include <tuple>
//#include <chrono>

using StackNode = std::tuple<KdTreeNode*, UINT, UINT>;

void KdTree::update()
{
	//auto timeStart = std::chrono::steady_clock::now();

	glm::vec3* pts = reinterpret_cast<glm::vec3*>(inputData->getVertexData());

	// Sort the points in the x, y, and z
	// Setup index arrays
	std::vector<UINT> indices(inputData->getPointCount());
	for (UINT i = 0; i < inputData->getPointCount(); i++)
	{
		indices[i] = i;
	}

	// Alternatively setup some sort of memory pool, maxsize
	std::vector<KdTreeNode*> ptrNodes;
	ptrNodes.push_back(new KdTreeNode(0, 0, -1, -1, -1.0f));

	std::stack<StackNode> nodeStack;
	nodeStack.push(StackNode(ptrNodes.back(), 0, static_cast<UINT>(indices.size())));
	while (true)
	{
		if (nodeStack.empty())
			break;
		const StackNode stackNode = nodeStack.top();
		KdTreeNode* node = std::get<0>(stackNode);
		const UINT start = std::get<1>(stackNode);
		const UINT end = std::get<2>(stackNode);
		nodeStack.pop();

		// Split
		const std::vector<UINT>::iterator begin = indices.begin() + start;
		const std::vector<UINT>::iterator last = indices.begin() + end;
		std::sort(begin, last, [indices, pts, node](UINT a, UINT b) { return pts[a][node->dim] > pts[b][node->dim]; });
		const UINT numVals = end - start;
		const UINT medianLocation = start + numVals / 2;
		const UINT medianIndex = indices[medianLocation];
		node->splitplane = pts[medianIndex][node->dim];

		// Address cast to int gets divided by size
		if (numVals < leafSize)
			continue;

		// Left
		node->leftId = static_cast<GLuint>(ptrNodes.size());
		const int childDim = (node->dim + 1) % 3;
		ptrNodes.push_back(new KdTreeNode(node->leftId, childDim, -1, -1, -1.0f));
		nodeStack.push({ ptrNodes.back(), start, medianLocation });

		// Right
		node->rightId = static_cast<GLuint>(ptrNodes.size());
		ptrNodes.push_back(new KdTreeNode(node->rightId, childDim, -1, -1, -1.0f));
		nodeStack.push({ ptrNodes.back(), medianLocation + 1, end });
	}

	// Copy it to stack/contingous spot in memory
	nodes = std::vector<KdTreeNode>(ptrNodes.size());
	for (size_t i = 0; i < ptrNodes.size(); i++)
	{
		nodes[i] = *ptrNodes[i];
		delete ptrNodes[i];
	}

	/*auto timeEnd = std::chrono::steady_clock::now();
	printf("Time: %f\n", std::chrono::duration<double, std::milli>(timeEnd - timeStart).count() / 1000.0);*/
}