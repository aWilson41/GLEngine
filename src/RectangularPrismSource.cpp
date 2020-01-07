#include "RectangularPrismSource.h"
#include "PolyData.h"

RectangularPrismSource::RectangularPrismSource() { outputData = new PolyData(); }
RectangularPrismSource::~RectangularPrismSource()
{
	if (outputData != nullptr)
		delete outputData;
}
void RectangularPrismSource::update()
{
	if (outputData == nullptr)
		return;

	// Points in the local coordinate system around origin
	glm::vec3 localPts[3] = { points[0] - origin, points[1] - origin, points[2] - origin };

	// Cube is is not indexed as it uses face normals

	// Vertices
	outputData->allocateVertexData(12, CellType::TRIANGLE);
	glm::vec3* vData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	glm::vec3 pts[8];
	pts[0] = origin;
	pts[1] = origin + localPts[0];
	pts[2] = origin + localPts[0] + localPts[1];
	pts[3] = origin + localPts[1];
	pts[4] = origin + localPts[2];
	pts[5] = origin + localPts[2] + localPts[0];
	pts[6] = origin + localPts[2] + localPts[0] + localPts[1];
	pts[7] = origin + localPts[2] + localPts[1];

	// Face 1
	// Triangle 1
	vData[0] = pts[0];
	vData[1] = pts[1];
	vData[2] = pts[2];
	// Triangle 2
	vData[3] = pts[0];
	vData[4] = pts[2];
	vData[5] = pts[3];
	// Face 2
	// Triangle 3
	vData[6] = pts[4];
	vData[7] = pts[0];
	vData[8] = pts[3];
	// Triangle 4
	vData[9] = pts[4];
	vData[10] = pts[3];
	vData[11] = pts[7];
	// Face 3
	// Triangle 5
	vData[12] = pts[3];
	vData[13] = pts[2];
	vData[14] = pts[6];
	// Triangle 6
	vData[15] = pts[3];
	vData[16] = pts[6];
	vData[17] = pts[7];
	// Face 4
	// Triangle 7
	vData[18] = pts[1];
	vData[19] = pts[5];
	vData[20] = pts[6];
	// Triangle 8
	vData[21] = pts[1];
	vData[22] = pts[6];
	vData[23] = pts[2];
	// Face 5
	// Triangle 9
	vData[24] = pts[4];
	vData[25] = pts[5];
	vData[26] = pts[1];
	// Triangle 10
	vData[27] = pts[4];
	vData[28] = pts[1];
	vData[29] = pts[0];
	// Face 6
	// Triangle 11
	vData[30] = pts[7];
	vData[31] = pts[6];
	vData[32] = pts[5];
	// Triangle 12
	vData[33] = pts[7];
	vData[34] = pts[5];
	vData[35] = pts[4];

	// Normals
	outputData->allocateNormalData();
	glm::vec3* nData = reinterpret_cast<glm::vec3*>(outputData->getNormalData());
	for (UINT i = 0; i < 36; i+=3)
	{
		glm::vec3 v1 = vData[i];
		glm::vec3 v2 = vData[i + 1];
		glm::vec3 v3 = vData[i + 2];
		nData[i] = glm::normalize(glm::cross(v1 - v2, v3 - v1));
	}
}