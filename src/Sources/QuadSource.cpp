#include "QuadSource.h"
#include "PolyData.h"

QuadSource::QuadSource() { outputData = std::make_shared<PolyData>(); }

void QuadSource::update()
{
	outputData->clear();

	outputData->allocateSharedVertexData(4, CellType::TRIANGLE);
	glm::vec3* vertexData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	// Defines a 1x1x1 plane
	vertexData[0] = origin;
	vertexData[1] = p1;
	vertexData[2] = p1 + p2 - origin;
	vertexData[3] = p2; // default along x

	outputData->allocateIndexData(8);
	GLuint* iData = outputData->getIndexData();
	iData[0] = 0; iData[1] = 1;
	iData[2] = 1; iData[3] = 2;
	iData[4] = 2; iData[5] = 3;
	iData[6] = 3; iData[7] = 0;
}