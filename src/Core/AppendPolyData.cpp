#include "AppendPolyData.h"
#include "MathHelper.h"
#include "PolyData.h"

AppendPolyData::AppendPolyData() { outputData = std::make_shared<PolyData>(); }

void AppendPolyData::update()
{
	if (inputData1 == nullptr && inputData2 == nullptr)
		return;

	if (inputData1 == nullptr)
	{
		outputData->copyFrom(inputData2);
		return;
	}
	if (inputData2 == nullptr)
	{
		outputData->copyFrom(inputData1);
		return;
	}

	outputData->clear();

	const UINT numVertices = inputData1->getVertexCount() + inputData2->getVertexCount();
	const UINT numIndices = inputData1->getIndexCount() + inputData2->getIndexCount();

	outputData->allocateVertexData(numVertices);
	outputData->allocateIndexData(numIndices, inputData1->getCellType());

	GLfloat* outputVertexData = outputData->getVertexData();
	UINT* outputIndexData = outputData->getIndexData();

	GLfloat* inputVertexData1 = inputData1->getVertexData();
	UINT* inputIndexData1 = inputData1->getIndexData();

	GLfloat* inputVertexData2 = inputData2->getVertexData();
	UINT* inputIndexData2 = inputData2->getIndexData();

	// Copy
	std::copy_n(inputVertexData1, inputData1->getVertexCount() * 3, outputVertexData);
	std::copy_n(inputVertexData2, inputData2->getVertexCount() * 3, outputVertexData + inputData1->getVertexCount() * 3);

	// 2nd polys indices needs to be shifted
	std::copy_n(inputIndexData1, inputData1->getIndexCount(), outputIndexData);
	for (UINT i = 0; i < inputData2->getIndexCount(); i++)
	{
		outputIndexData[i + inputData1->getIndexCount()] = inputIndexData2[i] + inputData1->getVertexCount();
	}
}