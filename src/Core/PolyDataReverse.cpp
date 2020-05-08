#include "PolyDataReverse.h"
#include "MathHelper.h"
#include "Geometry3D.h"

#include "PolyDataPointCloud.h"
#include "PolyData.h"
#include <Geometry2D.h>

PolyDataReverse::PolyDataReverse() { outputData = std::make_shared<PolyData>(); }

void PolyDataReverse::update()
{
	const UINT numInputPts = inputData->getVertexCount();
	const UINT numInputIndices = inputData->getIndexCount();

	outputData->clear();
	outputData->allocateVertexData(numInputPts);
	outputData->allocateIndexData(numInputIndices, inputData->getCellType());

	glm::vec3* outputVertexData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	glm::vec3* inputVertexData = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
	UINT* outputIndexData = outputData->getIndexData();
	UINT* inputIndexData = inputData->getIndexData();

	// If it has no indices, swap vertices
	if (numInputIndices == 0)
	{
		for (UINT i = 0; i < numInputPts; i += 3)
		{
			outputVertexData[i] = inputVertexData[i + 2];
			outputVertexData[i + 1] = inputVertexData[i + 1];
			outputVertexData[i + 2] = inputVertexData[i + 3];
		}
	}
	// If it has indices, swap indices
	else
	{
		for (UINT i = 0; i < numInputIndices; i += 3)
		{
			outputIndexData[i] = inputIndexData[i + 2];
			outputIndexData[i + 1] = inputIndexData[i + 1];
			outputIndexData[i + 2] = inputIndexData[i + 3];
		}
		std::copy_n(inputVertexData, numInputPts, outputVertexData);
	}
}