#include "RectangleSource.h"
#include "PolyData.h"

RectangleSource::RectangleSource() { outputData = std::make_shared<PolyData>(); }

void RectangleSource::update()
{
	outputData->clear();

	// Points in the local coordinate system around origin
	glm::vec3 localPts[2] = { points[0] - origin, points[1] - origin };

	// No duplicate points
	outputData->allocateVertexData(4);
	glm::vec3* vData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	vData[0] = origin;
	vData[1] = origin + localPts[0];
	vData[2] = origin + localPts[0] + localPts[1];
	vData[3] = origin + localPts[1];

	outputData->allocateIndexData(8, CellType::LINE);
	UINT* indices = outputData->getIndexData();
	indices[0] = 0;
	indices[1] = 1;

	indices[2] = 1;
	indices[3] = 2;

	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 3;
	indices[7] = 0;
}