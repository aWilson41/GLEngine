#include "PolyData.h"

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

glm::vec3 PolyData::getCenter() const
{
	glm::vec3 sum = glm::vec3(0.0f);
	glm::vec3* vertices = reinterpret_cast<glm::vec3*>(getVertexData());
	for (UINT i = 0; i < points.count; i++)
	{
		sum += vertices[i];
	}
	return sum / static_cast<GLfloat>(points.count);
}

void PolyData::setCellType(CellType type)
{
	cells.type = type;
	switch (cells.type)
	{
	case CellType::POINT:
		cells.cellCount = cells.indexCount;
		break;
	case CellType::LINE:
		cells.cellCount = cells.indexCount / 2;
		break;
	case CellType::TRIANGLE:
		cells.cellCount = cells.indexCount / 3;
		break;
	case CellType::QUAD:
		cells.cellCount = cells.indexCount / 4;
		break;
	default:
		break;
	}
}

void PolyData::allocateVertexData(UINT vertexCount)
{
	points.count = vertexCount;
	
	if (points.data != nullptr)
		delete[] points.data;
	points.data = new GLfloat[points.count * 3];
	std::fill_n(points.data, points.count * 3, 0.0f);
}
void PolyData::allocateIndexData(UINT indexCount, CellType type)
{
	cells.indexCount = indexCount;
	cells.type = type;

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
	std::fill_n(cells.data, indexCount, 0);
}
void PolyData::allocateNormalData()
{
	if (points.attrib[0] != nullptr)
		delete[] points.attrib[0];
	points.attrib[0] = new GLfloat[points.count * 3];
	std::fill_n(static_cast<GLfloat*>(points.attrib[0]), points.count * 3, 0.0f);
}
void PolyData::allocateTexCoords()
{
	if (points.attrib[1] != nullptr)
		delete[] points.attrib[1];
	points.attrib[1] = new GLfloat[points.count * 2];
	std::fill_n(static_cast<GLfloat*>(points.attrib[1]), points.count * 2, 0.0f);
}
void PolyData::allocateScalarData(UINT numComps)
{
	if (points.attrib[2] != nullptr)
		delete[] points.attrib[2];
	points.attrib[2] = new GLfloat[points.count * numComps];
	std::fill_n(static_cast<GLfloat*>(points.attrib[2]), points.count * numComps, 0.0f);
}

void PolyData::clear()
{
	points = PointData();
	cells = CellData();
}

void PolyData::copyFrom(std::shared_ptr<PolyData> sourcePolyData)
{
	clear();
	cells.indexCount = sourcePolyData->cells.indexCount;
	cells.cellCount = sourcePolyData->cells.cellCount;
	cells.type = sourcePolyData->cells.type;
	points.count = sourcePolyData->points.count;

	if (cells.indexCount != 0)
	{
		if (cells.data != nullptr)
			delete[] cells.data;
		cells.data = new UINT[cells.indexCount];
		std::copy_n(sourcePolyData->cells.data, cells.indexCount, cells.data);
	}
	if (points.count != 0)
	{
		if (points.data != nullptr)
			delete[] points.data;
		points.data = new float[points.count * 3];
		std::copy_n(sourcePolyData->points.data, points.count * 3, points.data);
	}
}