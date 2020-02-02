#include "CircleSource.h"
#include "PolyData.h"

CircleSource::CircleSource() { outputData = std::make_shared<PolyData>(); }

void CircleSource::update()
{
	outputData->clear();

	// Vertices
	const UINT vertexCount = div;
	outputData->allocateVertexData(vertexCount);
	glm::vec3* vData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());

	// Indices
	const UINT indexCount = div * 2;
	outputData->allocateIndexData(indexCount, CellType::LINE);
	GLuint* iData = outputData->getIndexData();

	const GLfloat stepSize = TWOPI / div;
	GLfloat theta = 0.0f;
	for (UINT i = 0; i < div - 1; i++)
	{
		vData[i] = glm::vec3(glm::cos(theta), glm::sin(theta), 0.0f) * radius + origin;
		iData[i * 2] = i;
		iData[i * 2 + 1] = i + 1;
		theta += stepSize;
	}

	const UINT lastIndex = vertexCount - 1;
	vData[lastIndex] = glm::vec3(glm::cos(theta), glm::sin(theta), 0.0f) * radius + origin;
	iData[lastIndex * 2] = lastIndex;
	iData[lastIndex * 2 + 1] = 0;
}