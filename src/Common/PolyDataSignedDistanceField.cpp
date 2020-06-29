#include "PolyDataSignedDistanceField.h"
#include "ImageData.h"
#include "ParallelFor.h"
#include "PolyData.h"

static void computeTriangleBounds(double* bounds, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	bounds[0] = FLOAT_MAX;
	bounds[1] = FLOAT_MIN;
	bounds[2] = FLOAT_MAX;
	bounds[3] = FLOAT_MIN;
	bounds[4] = FLOAT_MAX;
	bounds[5] = FLOAT_MIN;

	if (p1.x < bounds[0])
		bounds[0] = p1.x;
	if (p1.y < bounds[2])
		bounds[2] = p1.y;
	if (p1.z < bounds[4])
		bounds[4] = p1.z;
	if (p1.x > bounds[1])
		bounds[1] = p1.x;
	if (p1.y > bounds[3])
		bounds[3] = p1.y;
	if (p1.z > bounds[5])
		bounds[5] = p1.z;

	if (p2.x < bounds[0])
		bounds[0] = p2.x;
	if (p2.y < bounds[2])
		bounds[2] = p2.y;
	if (p2.z < bounds[4])
		bounds[4] = p2.z;
	if (p2.x > bounds[1])
		bounds[1] = p2.x;
	if (p2.y > bounds[3])
		bounds[3] = p2.y;
	if (p2.z > bounds[5])
		bounds[5] = p2.z;

	if (p3.x < bounds[0])
		bounds[0] = p3.x;
	if (p3.y < bounds[2])
		bounds[2] = p3.y;
	if (p3.z < bounds[4])
		bounds[4] = p3.z;
	if (p3.x > bounds[1])
		bounds[1] = p3.x;
	if (p3.y > bounds[3])
		bounds[3] = p3.y;
	if (p3.z > bounds[5])
		bounds[5] = p3.z;
}

static UINT calcIndex(UINT x, UINT y, UINT z, UINT width, UINT height) { return x + (y + height * z) * width; }

static bool computeNearTriangle(
	const glm::vec3& pt, const double tolerance,
	const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	// Compute normal of the triangle
	const glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));

	// Project points onto 
	const float distToPlane = glm::dot(p1 - pt, n);
	if (distToPlane < tolerance)
		return false;

	const float t = distToPlane / glm::dot(n, n);
	const glm::vec3 p = p1 + n * t;
	const glm::vec3 bCoords = MathHelp::baryCentric(p, p1, p2, p3);
	if (bCoords.x >= 0.0f && bCoords.y >= 0.0f && bCoords.z >= 0.0f)
		return true;
	else
		return false;
}

PolyDataSignedDistanceField::PolyDataSignedDistanceField()
{
	outputData = std::make_shared<ImageData>();
}

void PolyDataSignedDistanceField::update()
{
	outputData->clear();
	if (useReferenceImage)
	{
		UINT* refDim = referenceImage->getDimensions();
		dim[0] = refDim[0];
		dim[1] = refDim[1];
		dim[2] = refDim[2];
		double* refSpacing = referenceImage->getSpacing();
		spacing[0] = refSpacing[0];
		spacing[1] = refSpacing[1];
		spacing[2] = refSpacing[2];
		double* refOrigin = referenceImage->getOrigin();
		origin[0] = refOrigin[0];
		origin[1] = refOrigin[1];
		origin[2] = refOrigin[2];
	}
	outputData->allocate3DImage(dim, spacing, origin, 1, ScalarType::FLOAT_T);
	float* imgPtr = static_cast<float*>(outputData->getData());
	std::fill_n(imgPtr, dim[0] * dim[1] * dim[2], 0.0f);

	// Rasterize the triangle mesh into a binary mask
	// Use barycentric bounding box rasterizing (scaline is probably better)
	// slow, but can be faster for small triangles, also easy to implement
	glm::vec3* vertices = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	const UINT vertexCount = inputData->getVertexCount();
	UINT* indices = inputData->getIndexData();
	const UINT indexCount = inputData->getIndexCount();
	const UINT cellCount = inputData->getCellCount();

	const double spacingLength = glm::length(glm::vec3(spacing[0], spacing[1], spacing[2]));

	// For every cell compute the bounding box
	for (UINT i = 0; i < cellCount; i++)
	{
		double bounds[6];
		glm::vec3 p1 = vertices[indices[i * 3]];
		glm::vec3 p2 = vertices[indices[i * 3 + 1]];
		glm::vec3 p3 = vertices[indices[i * 3 + 2]];
		computeTriangleBounds(bounds, p1, p2, p3);

		// Now compute the extent
		UINT extent[6] = {
			static_cast<UINT>(bounds[0] / spacing[0]), static_cast<UINT>(bounds[1] / spacing[0]),
			static_cast<UINT>(bounds[2] / spacing[1]), static_cast<UINT>(bounds[3] / spacing[1]),
			static_cast<UINT>(bounds[4] / spacing[2]), static_cast<UINT>(bounds[5] / spacing[2]) };
		for (UINT z = extent[4]; z < extent[5] + 1; z++)
		{
			for (UINT y = extent[2]; y < extent[3] + 1; y++)
			{
				for (UINT x = extent[0]; x < extent[1] + 1; x++)
				{
					// Compute if this point is near the triangle
					glm::vec3 pt = glm::vec3(x * spacing[0], y * spacing[1], z * spacing[2]);
					if (computeNearTriangle(pt, spacingLength, p1, p2, p3))
						imgPtr[calcIndex(x, y, z, dim[0], dim[1])] = 1.0f;
				}
			}
		}
	}
}