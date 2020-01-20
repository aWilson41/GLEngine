#include "RectangleSource.h"
#include "PolyData.h"

RectangleSource::RectangleSource() { outputData = new PolyData(); }
RectangleSource::~RectangleSource()
{
	if (outputData != nullptr)
		delete outputData;
}
void RectangleSource::update()
{
	if (outputData == nullptr)
		return;

	// Points in the local coordinate system around origin
	glm::vec3 localPts[2] = { points[0] - origin, points[1] - origin };

	// No duplicate points
	outputData->allocateSharedVertexData(4, CellType::LINE);
	glm::vec3* vData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	vData[0] = origin;
	vData[1] = origin + localPts[0];
	vData[2] = origin + localPts[0] + localPts[1];
	vData[3] = origin + localPts[1];

	outputData->allocateIndexData(8);
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