#include "MathHelper.h"
#include "Geometry2D.h"
#include "Geometry3D.h"
#include <Eigen/SVD>
#include <tuple>

glm::mat4 MathHelp::rotateX(GLfloat radians)
{
	glm::mat4 results = glm::mat4(1.0f);
	results[1][1] = cos(radians);
	results[1][2] = -sin(radians);
	results[2][1] = sin(radians);
	results[2][2] = cos(radians);
	return results;
}
glm::mat4 MathHelp::rotateY(GLfloat radians)
{
	glm::mat4 results = glm::mat4(1.0f);
	results[0][0] = cos(radians);
	results[0][2] = sin(radians);
	results[2][0] = -sin(radians);
	results[2][2] = cos(radians);
	return results;
}
glm::mat4 MathHelp::rotateZ(GLfloat radians)
{
	glm::mat4 results = glm::mat4(1.0f);
	results[0][0] = cos(radians);
	results[0][1] = -sin(radians);
	results[1][0] = sin(radians);
	results[1][1] = cos(radians);
	return results;
}
glm::mat4 MathHelp::translate(GLfloat x, GLfloat y, GLfloat z)
{
	glm::mat4 results = glm::mat4(1.0f);
	results[3][0] = x;
	results[3][1] = y;
	results[3][2] = z;
	return results;
}
glm::mat4 MathHelp::translate(glm::vec3 t) { return translate(t.x, t.y, t.z); }
glm::mat4 MathHelp::scale(GLfloat s)
{
	return scale(s, s, s);
}
glm::mat4 MathHelp::scale(GLfloat x, GLfloat y, GLfloat z)
{
	glm::mat4 results = glm::mat4(1.0f);
	results[0][0] = x;
	results[1][1] = y;
	results[2][2] = z;
	return results;
}
glm::mat4 MathHelp::scale(glm::vec3 s) { return scale(s.x, s.y, s.z); }

glm::mat3 MathHelp::skewSym(glm::vec3 a) // ?
{
	glm::mat3 m;
	m[0][0] = 0.0f; m[0][1] = -a.z; m[0][2] = a.y;
	m[1][0] = a.z;  m[1][1] = 0.0f; m[1][2] = -a.x;
	m[2][0] = -a.y; m[2][1] = a.x;  m[2][2] = 0.0f;
	return m;
}

glm::mat3 MathHelp::rotateVectorToVector(glm::vec3 a, glm::vec3 b)
{
	glm::vec3 v = glm::cross(a, b);
	GLfloat s = glm::length(v);
	GLfloat c = glm::dot(a, b);
	glm::mat3 ss = skewSym(v);
	return glm::mat3(1.0f) + ss + ss * ss * (1 - c) / (s * s);
}

glm::quat MathHelp::Vec4ToQuat(glm::vec4 q)
{
	return glm::quat(q.w, q.x, q.y, q.z);
}
glm::vec4 MathHelp::QuatToVec4(glm::quat q)
{
	return glm::vec4(q.x, q.y, q.z, q.w);
}

// We define the 2d cross product as the length of the 3d ax * by - ay * bx
GLfloat MathHelp::cross(glm::vec2 a, glm::vec2 b) { return a.x * b.y - a.y * b.x; }

glm::vec2 MathHelp::slope(GLfloat theta) { return glm::vec2(glm::cos(theta), glm::sin(theta)); }

glm::vec2 MathHelp::perp(glm::vec2 a) { return glm::vec2(-a.y, a.x); }

GLfloat MathHelp::sqrDistance(glm::vec3 a, glm::vec3 b)
{
	glm::vec3 diff = b - a;
	return glm::dot(diff, diff);
}

glm::vec2 MathHelp::projAToB(glm::vec2 a, glm::vec2 b)
{
	GLfloat m = glm::length(b);
	return b * (glm::dot(a, b) / (m * m));
}
glm::vec3 MathHelp::projAToB(glm::vec3 a, glm::vec3 b)
{
	GLfloat m = glm::length(b);
	return b * (glm::dot(a, b) / (m * m));
}

// Triangles
GLfloat MathHelp::triangleAreaSigned(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) { return cross(p2 - p1, p3 - p1) * 0.5f; }
GLfloat MathHelp::triangleArea(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) { return abs(triangleAreaSigned(p1, p2, p3)); }
// Computes intersection point on triangle and then bary centric coordinate, returns vec4(u, v, w, intersectionDepth)
// If failed, returns vec4(0, 0, 0, max)
//glm::vec4 MathHelp::triangleRayIntersection(geom::Ray ray, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 n)
//{
//	// Ray plane intersection
//	GLfloat denom = glm::dot(ray.direction, n);
//	if (abs(denom) > 0.0001f) // epsilon
//	{
//		GLfloat t = glm::dot(p1 - ray.start, n) / denom;
//		glm::vec3 p = ray.start + ray.direction * t;
//
//		glm::vec3 bCoords = baryCentric(p, p1, p2, p3);
//
//		if (bCoords.x >= 0.0f && bCoords.y >= 0.0f && bCoords.z >= 0.0f)
//			return glm::vec4(bCoords.x, bCoords.y, bCoords.z, p.z);
//	}
//
//	return glm::vec4(0.0f, 0.0f, 0.0f, std::numeric_limits<GLfloat>::max());
//}

// Computes bary centric coordinates of point p in triangle a,b,c
glm::vec3 MathHelp::baryCentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	glm::vec2 v0 = b - a;
	glm::vec2 v1 = c - a;
	glm::vec2 v2 = p - a;
	GLfloat d00 = glm::dot(v0, v0);
	GLfloat d01 = glm::dot(v0, v1);
	GLfloat d11 = glm::dot(v1, v1);
	GLfloat d20 = glm::dot(v2, v0);
	GLfloat d21 = glm::dot(v2, v1);
	GLfloat denom = d00 * d11 - d01 * d01;
	GLfloat v = (d11 * d20 - d01 * d21) / denom;
	GLfloat w = (d00 * d21 - d01 * d20) / denom;
	GLfloat u = 1.0f - v - w;
	return glm::vec3(u, v, w);
}
glm::vec3 MathHelp::baryCentric(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 v0 = b - a;
	glm::vec3 v1 = c - a;
	glm::vec3 v2 = p - a;
	GLfloat d00 = glm::dot(v0, v0);
	GLfloat d01 = glm::dot(v0, v1);
	GLfloat d11 = glm::dot(v1, v1);
	GLfloat d20 = glm::dot(v2, v0);
	GLfloat d21 = glm::dot(v2, v1);
	GLfloat denom = d00 * d11 - d01 * d01;
	GLfloat v = (d11 * d20 - d01 * d21) / denom;
	GLfloat w = (d00 * d21 - d01 * d20) / denom;
	GLfloat u = 1.0f - v - w;
	return glm::vec3(u, v, w);
}

// Computes ray given screen position, width, height, fov, and near plane
//geom::Ray MathHelp::computeEyeRay(glm::vec2 pos, UINT width, UINT height, GLfloat fov, GLfloat nearZ)
//{
//	GLfloat aspectRatio = static_cast<GLfloat>(width) / height;
//	GLfloat s = 2.0f * tan(fov * 0.5f);
//	glm::vec3 start = glm::vec3((pos.x / width - 0.5f) * s /** aspectRatio*/, (pos.y / height - 0.5f) * s, 1.0f) * nearZ;
//	return geom::Ray(start, glm::normalize(start));
//}

glm::vec3 MathHelp::lerp(std::vector<std::tuple<GLfloat, glm::vec3>> colorFunc, GLfloat val)
{
	// Assumes colorFunc ordered by val1 in the tuple
	// If no colors specified return black
	if (colorFunc.size() < 0)
		return glm::vec3(0.0f, 0.0f, 0.0f);
	// If one color specified return that color
	if (colorFunc.size() < 1)
		return std::get<1>(colorFunc[0]);

	// If val is under 
	if (val <= std::get<0>(colorFunc[0]))
		return std::get<1>(colorFunc[0]);
	if (val >= std::get<0>(colorFunc.back()))
		return std::get<1>(colorFunc.back());

	for (UINT i = 0; i < colorFunc.size() - 1; i++)
	{
		GLfloat val1 = std::get<0>(colorFunc[i]);
		GLfloat val2 = std::get<0>(colorFunc[i + 1]);
		if (val >= val1 && val < val2)
			return lerp(std::get<1>(colorFunc[i]), std::get<1>(colorFunc[i + 1]), (val - val1) / (val2 - val1));
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 MathHelp::catmullRom(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, GLfloat t)
{
	GLfloat t2 = t * t;
	GLfloat t3 = t2 * t;
	return 0.5f * ((2.0f * p1) + (-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
}

glm::vec2 MathHelp::calcCentroid(glm::vec2* vertices, UINT count)
{
	glm::vec2 centroid(0.0f);
	GLfloat signedArea = 0.0f;
	glm::vec2 v1; // Current vertex
	glm::vec2 v2; // Next vertex
	GLfloat a = 0.0f; // Partial signed area
	UINT i = 0;
	for (i = 0; i < count - 1; ++i)
	{
		v1 = vertices[i];
		v2 = vertices[i + 1];
		a = cross(v1, v2);
		signedArea += a;
		centroid = (v1 + v2) * a;
	}

	// Do last vertex separately to avoid modulus operation in each iteration.
	v1 = vertices[i];
	v2 = vertices[0];
	a = cross(v1, v2);
	signedArea += a;
	signedArea *= 0.5f;

	return ((centroid + (v1 + v2) * a) / (6.0f * signedArea));
}
glm::vec3 MathHelp::calcCentroid(glm::vec3* vertices, UINT count)
{
	glm::vec3 centroid(0.0f);
	GLfloat signedArea = 0.0f;
	glm::vec3 v1; // Current vertex
	glm::vec3 v2; // Next vertex
	GLfloat a = 0.0f; // Partial signed area
	UINT i = 0;
	for (i = 0; i < count - 1; ++i)
	{
		v1 = vertices[i];
		v2 = vertices[i + 1];
		a = cross(v1, v2);
		signedArea += a;
		centroid = (v1 + v2) * a;
	}

	// Do last vertex separately to avoid modulus operation in each iteration.
	v1 = vertices[i];
	v2 = vertices[0];
	a = cross(v1, v2);
	signedArea += a;
	signedArea *= 0.5f;

	return ((centroid + (v1 + v2) * a) / (6.0f * signedArea));
}

geom2d::Rect MathHelp::get2dBounds(glm::vec2* vertices, UINT count)
{
	if (count == 0)
		return geom2d::Rect(glm::vec2(0.0f), glm::vec2(0.0f));
	GLfloat maxX, maxY, minX, minY;
	maxX = minX = vertices[0].x;
	maxY = minY = vertices[0].y;

	for (UINT i = 1; i < count; i++)
	{
		if (vertices[i].x > maxX)
			maxX = vertices[i].x;
		if (vertices[i].x < minX)
			minX = vertices[i].x;
		if (vertices[i].y > maxY)
			maxY = vertices[i].y;
		if (vertices[i].y < minY)
			minY = vertices[i].y;
	}
	glm::vec2 size = glm::vec2(maxX - minX, maxY - minY);
	return geom2d::Rect(glm::vec2(minX, minY) + size * 0.5f, size);
}
geom3d::Rect MathHelp::get3dBounds(glm::vec3* vertices, UINT count)
{
	if (count == 0)
		return geom3d::Rect(glm::vec3(0.0f), glm::vec3(0.0f));
	GLfloat maxX, maxY, maxZ, minX, minY, minZ;
	maxX = minX = vertices[0].x;
	maxY = minY = vertices[0].y;
	maxZ = minZ = vertices[0].z;

	for (UINT i = 1; i < count; i++)
	{
		if (vertices[i].x > maxX)
			maxX = vertices[i].x;
		if (vertices[i].x < minX)
			minX = vertices[i].x;
		if (vertices[i].y > maxY)
			maxY = vertices[i].y;
		if (vertices[i].y < minY)
			minY = vertices[i].y;
		if (vertices[i].z > maxZ)
			maxZ = vertices[i].z;
		if (vertices[i].z < minZ)
			minZ = vertices[i].z;
	}
	glm::vec3 size = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
	return geom3d::Rect(glm::vec3(minX, minY, minZ) + size * 0.5f, size);
}

void MathHelp::setData(glm::mat2x2& m, GLfloat m00, GLfloat m01, GLfloat m10, GLfloat m11)
{
	m[0][0] = m00;
	m[0][1] = m10;
	m[1][0] = m01;
	m[1][1] = m11;
}

void MathHelp::svd(glm::mat2x2 source, glm::mat2x2* u, glm::vec2* s, glm::mat2x2* v)
{
	Eigen::Matrix2f m;
	m(0, 0) = source[0][0];
	m(0, 1) = source[1][0];
	m(1, 0) = source[0][1];
	m(1, 1) = source[1][1];
	Eigen::JacobiSVD<Eigen::Matrix2f> svd(m, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::Matrix2f tmp1 = svd.matrixU();
	(*u)[0][0] = tmp1(0, 0);
	(*u)[1][0] = tmp1(1, 0);
	(*u)[0][1] = tmp1(0, 1);
	(*u)[1][1] = tmp1(1, 1);

	Eigen::Vector2f tmp2 = svd.singularValues();
	(*s)[0] = tmp2(0);
	(*s)[1] = tmp2(1);

	tmp1 = svd.matrixV();
	(*v)[0][0] = tmp1(0, 0);
	(*v)[1][0] = tmp1(1, 0);
	(*v)[0][1] = tmp1(0, 1);
	(*v)[1][1] = tmp1(1, 1);
}

void MathHelp::pd(glm::mat2x2 source, glm::mat2* r)
{
	glm::mat2 u, v;
	glm::vec2 s;
	svd(source, &u, &s, &v);
	glm::mat2 tmp = u * glm::transpose(v);

	(*r)[0][0] = tmp[0][0];
	(*r)[1][0] = tmp[1][0];
	(*r)[0][1] = tmp[0][1];
	(*r)[1][1] = tmp[1][1];
}

bool MathHelp::intersectTrianglePoint(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& pt)
{
	// Compute barycentric coordinates
	GLfloat u = 0.0f;
	GLfloat v = 0.0f;
	GLfloat w = 0.0f;
	glm::vec2 v0 = b - a;
	glm::vec2 v1 = c - a;
	glm::vec2 v2 = pt - a;
	GLfloat d00 = glm::dot(v0, v0);
	GLfloat d01 = glm::dot(v0, v1);
	GLfloat d11 = glm::dot(v1, v1);
	GLfloat d20 = glm::dot(v2, v0);
	GLfloat d21 = glm::dot(v2, v1);
	GLfloat denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
	if (u < 0.0f || u > 1.0f)
		return false;
	if (v < 0.0f || u + v > 1.0f)
		return false;
	return true;
}
bool MathHelp::intersectSegmentSegment(
	const glm::vec2& a1, const glm::vec2& a2,
	const glm::vec2& b1, const glm::vec2& b2,
	glm::vec2& intersectionPt)
{
	const glm::vec2 a = a2 - a1;
	const glm::vec2 b = b1 - b2;
	const glm::vec2 d = b1 - a1;

	const GLfloat det = MathHelp::cross(a, b);
	// Then lines are congruent (never intersect, may be colinear though)
	if (det == 0.0f)
		return false;

	const GLfloat invDet = 1.0f / det;
	const GLfloat r = MathHelp::cross(d, b) * invDet;
	const GLfloat s = MathHelp::cross(a, d) * invDet;

	if (r >= 0.0f && r <= 1.0f && s >= 0.0f && s <= 1.0f)
	{
		const glm::vec2 pos = a * r;
		intersectionPt = a1 + pos;
		return true;
	}
	else
		return false;
}