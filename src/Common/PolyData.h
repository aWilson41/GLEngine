#pragma once
#include "CellData.h"
#include "MathHelper.h"
#include "PointData.h"
#include "Types.h"
#include <vector>
#include <memory>

// Data = structure + attributes
// structure = topology + geometry
// PointData contains the geometry
// CellData contains the topology
// Attributes of each can be used to store things like normals, colors, other scalars, etc alongside geometry of topology
// 8 slots for attributes to keep them consistent in index.

// Actual container for poly data. Can store whatever geometry (points, lines, triangles, tetrahedrons, blah, blah blah)
// Contains functions to allocate normal, tex coords, and scalars as attributes in specific slots.
// By convention slots are utilized like so:
//  normal - 0, tex coords - 1, scalars - 2
class PolyData
{
public:
	UINT getVertexCount() const { return points.count; }
	UINT getCellCount() const { return cells.cellCount; }
	UINT getIndexCount() const { return cells.indexCount; }
	CellType getCellType() const { return cells.type; }

	GLfloat* getVertexData() const { return points.data; }
	GLfloat* getNormalData() const { return static_cast<GLfloat*>(points.attrib[0]); }
	GLfloat* getTexCoordData() const { return static_cast<GLfloat*>(points.attrib[1]); }
	GLfloat* getScalarData() const { return static_cast<GLfloat*>(points.attrib[2]); }
	GLuint* getIndexData() const { return cells.data; }
	
	// Computes area of a 2d XY polygon, temporary
	GLfloat getArea() const;
	glm::vec3 getCenter() const;
	float* getBounds() { return bounds; }

	void setPoints(PointData points) { this->points = points; }
	void setCells(CellData cells) { this->cells = cells; }
	void setCellType(CellType type);
	void setBounds(float left, float right, float bottom, float top, float back, float front)
	{
		this->bounds[0] = left;
		this->bounds[1] = right;
		this->bounds[2] = bottom;
		this->bounds[3] = top;
		this->bounds[4] = back;
		this->bounds[5] = front;
	}
	void setBounds(float* bounds) { setBounds(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]); }

public:
	// Allocate vertices and sets type
	void allocateVertexData(UINT vertexCount);
	void allocateIndexData(UINT indexCount, CellType type);
	void allocateNormalData();
	void allocateTexCoords();
	void allocateScalarData(UINT numComps);
	// Deletes all point and cell data
	void clear();
	// Doesn't copy attributes
	void copy(std::shared_ptr<PolyData> sourcePolyData);
	// Recompute the bounds
	void updateBounds();

protected:
	PointData points;
	CellData cells;
	float bounds[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};