#include "KdTree.h"
#include "PolyData.h"
#include <stack>
#include <tuple>
//#include <chrono>

void KdTree::update()
{
	//auto timeStart = std::chrono::steady_clock::now();

	std::vector<KdTreeNode*> ptrNodes;
	ptrNodes.push_back(new KdTreeNode(0, 0, -1, -1, -1.0f, 0, numPts, -1));

	// Setup index array
	indices.resize(numPts);
	for (UINT i = 0; i < numPts; i++)
	{
		indices[i] = i;
	}

	// Compute bounds whilst computing kdtree
	// Within the first 3 cycles of dimension we can know the bounds
	GLfloat bounds[6] = {
		std::numeric_limits<GLfloat>::max(),
		std::numeric_limits<GLfloat>::min(),
		std::numeric_limits<GLfloat>::max(),
		std::numeric_limits<GLfloat>::min(),
		std::numeric_limits<GLfloat>::max(),
		std::numeric_limits<GLfloat>::min() };
	for (UINT i = 0; i < numPts; i++)
	{
		glm::vec3 pt = accessorFunc(i);
		bounds[0] = std::min(pt.x, bounds[0]);
		bounds[1] = std::max(pt.x, bounds[1]);
		bounds[2] = std::min(pt.y, bounds[2]);
		bounds[3] = std::max(pt.y, bounds[3]);
		bounds[4] = std::min(pt.z, bounds[4]);
		bounds[5] = std::max(pt.z, bounds[5]);
	}

	// Sort using accessor function pointer
	std::stack<KdTreeNode*> nodeStack;
	nodeStack.push(ptrNodes.back());
	while (true)
	{
		if (nodeStack.empty())
			break;
		KdTreeNode* node = nodeStack.top();
		nodeStack.pop();

		const UINT numVals = node->end - node->start;
		if (numVals < leafSize)
			continue;

		// Split
		const std::vector<UINT>::iterator start = indices.begin() + node->start;
		const std::vector<UINT>::iterator end = indices.begin() + node->end;

		std::sort(start, end, [&](UINT a, UINT b) { return accessorFunc(a)[node->dim] < accessorFunc(b)[node->dim]; });

		const UINT medianLocation = node->start + numVals / 2;
		const UINT medianIndex = indices[medianLocation];
		node->splitplane = accessorFunc(medianIndex)[node->dim];
		const int childDim = (node->dim + 1) % 3;

		// Stop processing if sphere doesn't fit in bounds

		// Left
		node->leftId = static_cast<GLuint>(ptrNodes.size());
		ptrNodes.push_back(new KdTreeNode(node->leftId, childDim, -1, -1, -1.0f, node->start, medianLocation, node->id));
		nodeStack.push(ptrNodes.back());

		// Right
		node->rightId = static_cast<GLuint>(ptrNodes.size());
		ptrNodes.push_back(new KdTreeNode(node->rightId, childDim, -1, -1, -1.0f, medianLocation + 1, node->end, node->id));
		nodeStack.push(ptrNodes.back());
	}

	// Copy it to stack/contingous spot in memory (Alternatively we could setup some sort of memory pool, maxsize)
	nodes = std::vector<KdTreeNode>(ptrNodes.size());
	for (size_t i = 0; i < ptrNodes.size(); i++)
	{
		nodes[i] = *ptrNodes[i];
		delete ptrNodes[i];
	}

	/*auto timeEnd = std::chrono::steady_clock::now();
	printf("Time: %f\n", std::chrono::duration<double, std::milli>(timeEnd - timeStart).count() / 1000.0);*/
}