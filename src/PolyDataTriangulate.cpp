#include "PolyDataTriangulate.h"
#include "PolyData.h"
#include <list>

// With ear clipping we iterate sets of 3 vertices testing if they are a triangle on the outside of the surface
// We test if its convex/concave and clip if convex. IE: If the center point of the 3 points along the surface lies
// beneath the angle between 1 and 2. (negative area, wrong winding). Do this over and over again until we are left
// with a single triangle
static void earClipTriangulate(PolyData* inputData, PolyData* outputData)
{
	glm::vec3* inputVertexData = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	const UINT numInputPts = inputData->getPointCount();

	// Output polygon vertices
	std::vector<glm::vec3> outputVertices;
	// Setup a list of indices for fast removal as we snip
	std::vector<UINT> indices;
	for (UINT i = 0; i < numInputPts; i++)
	{
		indices.push_back(i);
	}
	UINT i = 0;
	UINT numPts = numInputPts;

	// Iterate sets of 3 points
	while (numPts != 3)
	{
		UINT i1 = indices[i % numPts];
		UINT i2 = indices[(i + 1) % numPts];
		UINT i3 = indices[(i + 2) % numPts];
		glm::vec3 p1 = inputVertexData[i1];
		glm::vec3 pt = inputVertexData[i2];
		glm::vec3 p3 = inputVertexData[i3];

		// Figure out if p2 is concave
		GLfloat signedArea = MathHelp::triangleAreaSigned(p1, pt, p3);
		if (signedArea > 0.0f)
		{
			// If it contains a point from the existing polygon then bail on this pt
			bool containsPt = false;
			for (unsigned int j = 0; j < numPts; j++)
			{
				UINT i4 = indices[j];
				if (i4 != i1 && i4 != i2 && i4 != i3 && MathHelp::intersectTrianglePoint(p1, pt, p3, inputVertexData[i4]))
				{
					containsPt = true;
					break;
				}
			}
			if (!containsPt)
			{
				outputVertices.push_back(p1);
				outputVertices.push_back(pt);
				outputVertices.push_back(p3);
				indices.erase(indices.begin() + (i + 1) % numPts);
				numPts--;
			}
		}

		i++;
	}
	// Add the last triangle
	outputVertices.push_back(inputVertexData[indices[0]]);
	outputVertices.push_back(inputVertexData[indices[1]]);
	outputVertices.push_back(inputVertexData[indices[2]]);

	outputData->allocateVertexData(static_cast<UINT>(outputVertices.size()) / 3, CellType::TRIANGLE);
	std::copy_n(reinterpret_cast<GLfloat*>(outputVertices.data()), outputVertices.size() * 3, outputData->getVertexData());
}

// With monotone triangulation we line sweep the polygon cutting them to partition into a set of monotone polygons
// which are then trivial to triangulate separately. It is faster than ear clipping
static void monotoneTriangulate(PolyData* inputData, PolyData* outputData)
{
	glm::vec3* inputVertexData = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	const UINT numInputPts = inputData->getPointCount();

	// Line sweep along y
	std::vector<UINT> indices;
	for (UINT i = 0; i < numInputPts; i++)
	{
		indices.push_back(i);
	}
	std::sort(indices.begin(), indices.end(), [&](UINT a, UINT b) { return inputVertexData[a].y < inputVertexData[b].y; });
	// The top is the starting vertex
	//UINT leftCurrIndex = (indices[0] + 1) % numInputPts;
	//UINT leftPrevIndex = indices[0];
	//UINT rightCurrIndex = (indices[0] - 1) % numInputPts;
	//UINT rightPrevIndex = indices[0];

	//while (true)
	//{

	//	// Lastly push back the indices and increment
	//	leftPrevIndex = leftCurrIndex;
	//	leftCurrIndex = (leftCurrIndex + 1) % numInputPts;
	//	rightPrevIndex = rightCurrIndex;
	//	rightCurrIndex = (rightCurrIndex + 1) % numInputPts;
	//}
}

static void delaunayTriangulate(PolyData* inputData, PolyData* outputData)
{
	glm::vec3* inputVertexData = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	const UINT numInputPts = inputData->getPointCount();
}

PolyDataTriangulate::PolyDataTriangulate() { outputData = new PolyData(); }
PolyDataTriangulate::~PolyDataTriangulate() { delete outputData; }

void PolyDataTriangulate::update()
{
	if (outputData != nullptr)
		delete outputData;
	outputData = new PolyData();

	if (type == TriangulateType::EARCLIP)
		earClipTriangulate(inputData, outputData);
	else if (type == TriangulateType::MONOTONE)
		monotoneTriangulate(inputData, outputData);
	else if (type == TriangulateType::DELAUNAY)
		delaunayTriangulate(inputData, outputData);
}