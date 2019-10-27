#pragma once
#include "CellData.h"
#include "MathHelper.h"
#include "PointData.h"
#include "Types.h"
#include <vector>
// Plans:
//    - Add unlimited attributes but specific slots. So an attribute could be set as scalars or normals rather than relying
//   on attribute position to tell.
//    - Inherit DataSet base class

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
	UINT getPointCount() const { return points.count; }
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

	// Allocate vertices by cells. Resulting amount of vertices depend on cell type
	void allocateVertexData(UINT cellCount, CellType type);
	// Allocate vertices by vertex count (usually for indexing later)
	void allocateSharedVertexData(UINT vertexCount, CellType type);
	void allocateIndexData(UINT indexCount);
	void allocateNormalData();
	void allocateTexCoords();
	void allocateScalarData(UINT numComps);

protected:
	PointData points;
	CellData cells;
};