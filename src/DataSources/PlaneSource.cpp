#include "PlaneSource.h"
#include "PolyData.h"

PlaneSource::PlaneSource() { outputData = std::make_shared<PolyData>(); }

void PlaneSource::update()
{
	outputData->clear();

	outputData->allocateVertexData(4);
	glm::vec3* vertexData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	// Defines a 1x1x1 plane
	vertexData[0] = origin;
	vertexData[1] = p2;
	vertexData[2] = p1 + p2 - origin;
	vertexData[3] = p1; // default along x

	outputData->allocateIndexData(6, CellType::TRIANGLE);
	GLuint* iData = outputData->getIndexData();
	iData[0] = 2; iData[1] = 1; iData[2] = 0;
	iData[3] = 3; iData[4] = 2; iData[5] = 0;

	outputData->allocateNormalData();
	glm::vec3* normalData = reinterpret_cast<glm::vec3*>(outputData->getNormalData());
	normalData[0] = normalData[1] = normalData[2] = normalData[3] = glm::vec3(0.0f, 1.0f, 0.0f);

	outputData->allocateTexCoords();
	glm::vec2* texCoordData = reinterpret_cast<glm::vec2*>(outputData->getTexCoordData());
	texCoordData[0] = glm::vec2(0.0f, 0.0f);
	texCoordData[1] = glm::vec2(0.0f, 1.0f);
	texCoordData[2] = glm::vec2(1.0f, 1.0f);
	texCoordData[3]  = glm::vec2(1.0f, 0.0f);
}