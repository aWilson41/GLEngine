#include "PolyData.h"

void PolyData::allocateVertexData(UINT cellCount, CellType type)
{
	cells.cellCount = cellCount;
	if (points.data != nullptr)
		delete[] points.data;

	switch (type)
	{
	case CellType::POINT:
		points.count = cellCount;
		break;
	case CellType::LINE:
		points.count = cellCount * 2;
		break;
	case CellType::TRIANGLE:
		points.count = cellCount * 3;
		break;
	case CellType::QUAD:
		points.count = cellCount * 4;
		break;
	default:
		break;
	}

	points.data = new GLfloat[points.count * 3];
	memset(points.data, 0, sizeof(GLfloat) * points.count * 3);
}
void PolyData::allocateSharedVertexData(UINT vertexCount, CellType type)
{
	points.count = vertexCount;
	cells.type = type;
	if (points.data != nullptr)
		delete[] points.data;
	points.data = new GLfloat[points.count * 3];
	memset(points.data, 0, sizeof(GLfloat) * points.count * 3);
}
void PolyData::allocateIndexData(UINT indexCount)
{
	cells.indexCount = indexCount;

	switch (cells.type)
	{
	case CellType::POINT:
		cells.cellCount = indexCount;
		break;
	case CellType::LINE:
		cells.cellCount = indexCount / 2;
		break;
	case CellType::TRIANGLE:
		cells.cellCount = indexCount / 3;
		break;
	case CellType::QUAD:
		cells.cellCount = indexCount / 4;
		break;
	default:
		break;
	}
	
	if (cells.data != nullptr)
		delete[] cells.data;
	cells.data = new GLuint[indexCount];
	memset(cells.data, 0, sizeof(GLuint) * cells.indexCount);
}
void PolyData::allocateNormalData()
{
	if (points.attrib[0] != nullptr)
		delete[] points.attrib[0];
	points.attrib[0] = new GLfloat[points.count * 3];
	memset(points.attrib[0], 0, sizeof(GLfloat) * points.count * 3);
}
void PolyData::allocateTexCoords()
{
	if (points.attrib[1] != nullptr)
		delete[] points.attrib[1];
	points.attrib[1] = new GLfloat[points.count * 2];
	memset(points.attrib[1], 0, sizeof(GLfloat) * points.count * 2);
}
void PolyData::allocateScalarData(UINT numComps)
{
	if (points.attrib[2] != nullptr)
		delete[] points.attrib[2];
	points.attrib[2] = new GLfloat[points.count * numComps];
	memset(points.attrib[2], 0, sizeof(GLfloat) * points.count * numComps);
}

GLfloat PolyData::getArea() const
{
	GLfloat area = 0.0f;
	glm::vec3* vertices = reinterpret_cast<glm::vec3*>(getVertexData());
	for (UINT i = 0; i < points.count - 1; i++)
	{
		area += MathHelp::cross(glm::vec2(vertices[i]), glm::vec2(vertices[i + 1]));
	}
	area += MathHelp::cross(glm::vec2(vertices[points.count - 1]), glm::vec2(vertices[0]));
	return area * 0.5f;
}