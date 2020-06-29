#include "PolyDataSubdivision.h"
#include "MathHelper.h"
#include "PolyData.h"

PolyDataSubdivision::PolyDataSubdivision() { outputData = std::make_shared<PolyData>(); }

void PolyDataSubdivision::update()
{
	if (inputData == nullptr)
		return;

	outputData->clear();

	if (type == SubdivisionType::LINEAR)
	{
		if (inputData->getCellType() == CellType::LINE)
		{
			const UINT inputVertexCount = inputData->getVertexCount();
			const UINT inputIndexCount = inputData->getIndexCount();
			const UINT inputCellCount = inputData->getCellCount();
			glm::vec3* inputVertices = reinterpret_cast<glm::vec3*>(inputData->getVertexData());
			UINT* inputIndices = inputData->getIndexData();

			// Add 1 pt for each cell
			outputData->allocateVertexData(inputVertexCount + inputCellCount * subdivCount);
			outputData->allocateIndexData(inputIndexCount + 2 * inputCellCount * subdivCount, CellType::LINE);
			glm::vec3* outputVertices = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
			UINT* outputIndices = outputData->getIndexData();

			// Assumes sequential connection of cells

			// Generate vertices
			UINT vertexIter = 0;
			const UINT numVertsPerLine = subdivCount + 2;
			for (UINT i = 0; i < inputCellCount; i++)
			{
				// Get input data
				const UINT i1 = inputIndices[i * 2];
				const UINT i2 = inputIndices[i * 2 + 1];
				const glm::vec3& v1 = inputVertices[i1];
				const glm::vec3& v2 = inputVertices[i2];
				
				const glm::vec3 diff = v2 - v1;
				for (UINT j = 0; j < numVertsPerLine - 1; j++)
				{
					const float t = static_cast<float>(j) / (numVertsPerLine - 1);
					outputVertices[vertexIter++] = v1 + diff * t;
				}
			}

			// Generate indices
			for (UINT i = 0; i < outputData->getCellCount(); i++)
			{
				outputIndices[i * 2] = i;
				outputIndices[i * 2 + 1] = i + 1;
			}
			outputIndices[outputData->getIndexCount() - 1] = 0;
		}
	}
}