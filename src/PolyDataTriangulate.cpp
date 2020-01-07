#include "PolyDataTriangulate.h"
#include "PolyData.h"
#include <list>

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

	outputData->allocateVertexData(outputVertices.size() / 3, CellType::TRIANGLE);
	std::copy_n(reinterpret_cast<GLfloat*>(outputVertices.data()), outputVertices.size() * 3, outputData->getVertexData());
}

static void monotoneTriangulate(PolyData* inputData, PolyData* outputData)
{
	glm::vec3* inputVertexData = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	const UINT numInputPts = inputData->getPointCount();
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

	// With ear clipping we essentially trip to clip off triangles from sets of 3 points on the surface
	// If the clipped triangle is convex (positive area) then it can be clipped, if not skip it and come back
	if (type == TriangulateType::EARCLIP)
		earClipTriangulate(inputData, outputData);
	else if (type == TriangulateType::MONOTONE)
		monotoneTriangulate(inputData, outputData);
	else if (type == TriangulateType::DELAUNAY)
		delaunayTriangulate(inputData, outputData);
}