//#include "QuadTree.h"
//#include "Geometry3D.h"
//
//static UINT computeQuadrant(const glm::vec2& pt, const glm::vec2& quadCellCenter)
//{
//	// Compute quadrant
//	UINT qi = 0;
//	if (pt.x >= quadCellCenter.x)
//		qi++;
//	if (pt.y >= quadCellCenter.y)
//		qi += 2;
//	return qi;
//}
//
//void QuadTree::init(glm::vec3* pts, UINT numPts, geom2d::Rect* initBounds)
//{
//	setAccessor([&](const UINT& i) { return pts[i]; }, numPts);
//
//	// If no bounds provided set to the bounds of the points
//	if (initBounds == nullptr)
//	{
//		geom3d::Rect boundsRect = MathHelp::get3dBounds(pts, numPts);
//		this->initBounds = geom2d::Rect(boundsRect.pos, boundsRect.size());
//	}
//	// If the user provided bounds, use that
//	else
//		this->initBounds = *initBounds;
//
//	const UINT rootId = 0;
//	nodes.push_back(QuadTreeNode(rootId, this->initBounds));
//	roots[glm::ivec2(0, 0)] = rootId;
//
//	indices.resize(numPts);
//
//	update();
//}
//
//// Update the quadtree
//void QuadTree::update()
//{
//	const glm::vec2 rootSize = initBounds.size();
//	const glm::vec2 shift = initBounds.bottomLeft();
//
//	// Clear all roots and nodes
//	nodes.clear();
//	roots.clear();
//
//	// For every point
//	nodes.reserve(numPts / 3);
//	for (UINT i = 0; i < numPts; i++)
//	{
//		// Hash and determine the root node to use
//		const glm::vec2& pt = glm::vec2(accessorFunc(i));
//		const glm::ivec3& hashPt = glm::ivec3((pt - shift) / rootSize, 0);
//		const UINT rootNodeIndex = roots[hashPt];
//		QuadTreeNode& rootNode = nodes[rootNodeIndex];
//
//		// Assign all points to the root
//		indices[i] = rootNodeIndex;
//		UINT currNodeIndex = rootNodeIndex;
//
//		// Iterate down the tree to the leaf this point belongs in
//		while (true)
//		{
//			QuadTreeNode& currNode = nodes[currNodeIndex];
//			// Compute quadrant
//			const UINT qi = computeQuadrant(pt, currNode.rect.pos);
//			// If leaf
//			if (currNode.childIds[0] == -1)
//			{
//				// Insert point
//				indices[currNode.childIds[0]] = currNodeIndex;
//				currNode.indices.push_back(i);
//				const glm::vec2 halfSize = currNode.rect.size() * 0.5f;
//				// If we exceed max count, subdivide into 4 new cells
//				if (currNode.indices.size() >= maxCount && halfSize.x > minEdgeSize && halfSize.y > minEdgeSize)
//				{
//					const glm::vec2 dx = currNode.rect.size() * 0.25f;
//
//					currNode.childIds[0] = static_cast<int>(nodes.size());
//					QuadTreeNode& botLeftNode = QuadTreeNode(currNode.childIds[0], geom2d::Rect(currNode.rect.pos - dx, halfSize));
//					nodes.push_back(botLeftNode);
//
//					currNode.childIds[1] = static_cast<int>(nodes.size());
//					QuadTreeNode& botRightNode = QuadTreeNode(currNode.childIds[1], geom2d::Rect(currNode.rect.pos + glm::vec2(dx.x, -dx.y), halfSize));
//					nodes.push_back(botRightNode);
//
//					currNode.childIds[2] = static_cast<int>(nodes.size());
//					QuadTreeNode& topLeftNode = QuadTreeNode(currNode.childIds[2], geom2d::Rect(currNode.rect.pos + glm::vec2(-dx.x, dx.y), halfSize));
//					nodes.push_back(topLeftNode);
//
//					currNode.childIds[3] = static_cast<int>(nodes.size());
//					QuadTreeNode& topRightNode = QuadTreeNode(currNode.childIds[3], geom2d::Rect(currNode.rect.pos + dx, halfSize));
//					nodes.push_back(topRightNode);
//
//					// Sort the points down the tree
//					for (std::list<UINT>::iterator j = currNode.indices.begin(); j != currNode.indices.end(); j++)
//					{
//						const glm::vec2& ptj = glm::vec2(accessorFunc(*j));
//						const UINT qj = computeQuadrant(ptj, currNode.rect.pos);
//						const UINT nodeId = currNode.childIds[qj];
//						nodes[nodeId].indices.push_back(*j);
//						indices[*j] = nodeId;
//					}
//					currNode.indices.clear();
//				}
//				break;
//			}
//			else
//			{
//				// Iterate down
//				currNodeIndex = currNode.childIds[qi];
//			}
//		}
//	}
//}