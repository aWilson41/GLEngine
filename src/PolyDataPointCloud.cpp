#include "PolyDataPointCloud.h"
#include "PolyData.h"
#include <Geometry2D.h>
#include <random>
#include <time.h>

static bool isPointInPolygon2d(glm::vec3* vertices, UINT numVerts, glm::vec3 pt)
{
	// The line segment test would be faster, but this is super easy
	bool result = false;
	for (UINT i = 0, j = numVerts - 1; i < numVerts; j = i++)
	{
		glm::vec3& vi = vertices[i];
		glm::vec3& vj = vertices[j];
		if ((vi.y > pt.y) != (vj.y > pt.y) && (pt.x < (vj.x - vi.x) * (pt.y - vi.y) / (vj.y - vi.y) + vi.x))
			result = !result;
	}
	return result;
}
static bool isPointInPolygon3d(glm::vec3* vertices, UINT* indices, UINT numIndices, glm::vec3 pt)
{
	// For every face
	for (UINT i = 0; i < numIndices; i += 3)
	{
		glm::vec3 v1 = vertices[indices[i]];
		glm::vec3 v2 = vertices[indices[i + 1]];
		glm::vec3 v3 = vertices[indices[i + 2]];

		glm::vec3 diff1 = v2 - v1;
		glm::vec3 diff2 = v3 - v1;
		glm::vec3 n = glm::normalize(glm::cross(diff1, diff2));

		glm::vec3 p2f = v1 - pt;
		GLfloat d = glm::dot(p2f, n);
		d /= glm::length(p2f);

		// Epsilon for boundaries
		if (d < -1e-15f)
			return false;
	}

	return true;
}

PolyDataPointCloud::PolyDataPointCloud() { outputData = new PolyData(); }
PolyDataPointCloud::~PolyDataPointCloud() { delete outputData; }

void PolyDataPointCloud::update()
{
	// Vertices
	outputData->allocateSharedVertexData(numPts, CellType::POINT);
	glm::vec3* outputVertexData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());

	glm::vec3* inputVertexData = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	const UINT numInputPts = inputData->getPointCount();

	// Assume line segments
	const geom3d::Rect rect = MathHelp::get3dBounds(inputVertexData, numInputPts);
	bounds[0] = rect.pos.x - rect.extent.x;
	bounds[1] = rect.pos.x + rect.extent.x;
	bounds[2] = rect.pos.y - rect.extent.y;
	bounds[3] = rect.pos.y + rect.extent.y;
	bounds[4] = rect.pos.z - rect.extent.z;
	bounds[5] = rect.pos.z + rect.extent.z;
	if (use2d)
	{
		// Just a rectangle hit or miss strategy. Maybe later I'll come up with a better scheme but for small polygons
		// this is sufficient for me
		if (numInputPts < 3)
			return;

		if (rect.size().z < 0.00001f)
		{
			geom2d::Rect aabb;
			aabb.pos = rect.pos;
			aabb.extent = rect.extent;

			const std::uniform_int_distribution<std::mt19937::result_type> random(0, UINT_MAX);
			std::mt19937 rng = std::mt19937(static_cast<UINT>(time(NULL)));
			UINT count = 0;
			while (count < numPts)
			{
				// Generate a random point
				glm::vec2 newPt = glm::vec2(static_cast<GLfloat>(random(rng)), static_cast<GLfloat>(random(rng)));
				// Change random to [-1, 1]
				newPt = newPt / (UINT_MAX * 0.5f) - 1.0f;
				// Change random to [-(width or height) / 2, (width or height) / 2] and add center
				newPt = newPt * aabb.extent + aabb.pos;

				// Check if the point lies in the polygon
				if (isPointInPolygon2d(inputVertexData, numInputPts, glm::vec3(newPt, rect.pos.z)))
					outputVertexData[count++] = glm::vec3(newPt, 0.0f);
			}

			// Iteratively move points out of each other
			const GLfloat r2 = radius * radius;
			for (UINT i = 0; i < numIterations; i++)
			{
				for (UINT j = 0; j < numPts; j++)
				{
					for (UINT k = j + 1; k < numPts; k++)
					{
						glm::vec3& p1 = outputVertexData[j];
						glm::vec3& p2 = outputVertexData[k];
						const glm::vec3 diff = p2 - p1;
						const GLfloat sqrLength = glm::dot(diff, diff);

						if (sqrLength < r2)
						{
							GLfloat dist = glm::sqrt(sqrLength);
							glm::vec3 n = diff / dist;

							glm::vec3 halfResolve = n * (dist - radius) * 0.5f;
							glm::vec3 resolvedPt1 = p1 + halfResolve;
							glm::vec3 resolvedPt2 = p2 - halfResolve;
							if (isPointInPolygon2d(inputVertexData, numInputPts, resolvedPt1))
								p1 = resolvedPt1;
							if (isPointInPolygon2d(inputVertexData, numInputPts, resolvedPt2))
								p2 = resolvedPt2;
						}
					}
				}
			}
		}
		else
		{
			// Compute a basis, parallel to the plane
			//const glm::vec3 diff1 = inputVertexData[1] - inputVertexData[0];
			//const glm::vec3 diff2 = inputVertexData[2] - inputVertexData[0];
			//const glm::vec3 i = glm::normalize(diff1);
			//// Gives the plane normal
			//const glm::vec3 perp = glm::normalize(glm::cross(i, diff2));
			//const glm::vec3 j = glm::normalize(glm::cross(i, perp));

			//// Now we have two perpendicular vectors i, j on the plane in 3d space

			//// Compute bounds with respect to i and j
			//glm::vec3 origin = inputVertexData[0];
			//GLfloat bounds[4] = { origin.x, origin.x, origin.y, origin.y };
			//for (UINT index = 1; index < numInputPts; index++)
			//{
			//	glm::vec3 ptMove = inputVertexData[index] - origin;

			//	// Project onto i to get x
			//	glm::vec2 planeCoords = glm::vec2(glm::dot(i, ptMove), glm::dot(j, ptMove));

			//	if ()
			//}

			//// Project the 3d points to 2d plane
			//glm::vec2* outPts = new glm::vec2[numInputPts];
			//MathHelp::projToPlane(inputVertexData, outPts, numInputPts, inputVertexData[0], i, j);

			//const geom2d::Rect aabb = MathHelp::get2dBounds(outPts, numInputPts);
			//const glm::vec2 pos = aabb.pos;
			//const glm::vec2 extent = aabb.extent;
			//const glm::vec2 size = extent * 2.0f;

			//const UINT maxUint = std::numeric_limits<UINT>::max();
			//const std::uniform_int_distribution<std::mt19937::result_type> random(0, maxUint);
			//const std::mt19937 rng = std::mt19937(static_cast<UINT>(time(NULL)));
			//UINT count = 0;
			//while (count < numPts)
			//{
			//	// Generate a random point
			//	glm::vec2 newPt = glm::vec2(static_cast<GLfloat>(random(rng)), static_cast<GLfloat>(random(rng)));
			//	// Change random to [-1, 1]
			//	newPt = newPt / (maxUint * 0.5f) - 1.0f;
			//	// Change random to [-(width or height) / 2, (width or height) / 2] and add center
			//	newPt = newPt * extent + pos;

			//	// Check if the point lies in the polygon
			//	if (MathHelp::isPointInPolygon2d(outPts, numInputPts, newPt))
			//		outputVertexData[count++] = ;
			//}

			//delete[] outPts;
		}
	}
	// Assume triangles
	else
	{
		if (inputData->getCellType() != CellType::TRIANGLE)
			return;

		UINT* indices = inputData->getIndexData();
		const UINT numIndices = inputData->getIndexCount();

		const std::uniform_int_distribution<std::mt19937::result_type> random(0, UINT_MAX);
		std::mt19937 rng = std::mt19937(static_cast<UINT>(time(NULL)));
		UINT count = 0;
		while (count < numPts)
		{
			// Generate a random point
			glm::vec3 newPt = glm::vec3(static_cast<GLfloat>(random(rng)), static_cast<GLfloat>(random(rng)), static_cast<GLfloat>(random(rng)));
			// Change random to [-1, 1]
			newPt = newPt / (UINT_MAX * 0.5f) - 1.0f;
			// Change random to [-(width or height) / 2, (width or height) / 2] and add center
			newPt = newPt * rect.extent + rect.pos;

			// Check if the point lies in the polygon
			if (isPointInPolygon3d(inputVertexData, indices, numIndices, newPt))
				outputVertexData[count++] = newPt;
		}
	}
}