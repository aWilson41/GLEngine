#pragma once
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math.h>
#include <vector>

using UINT = unsigned int;

// Conversion of Math double of PI to float PI
const GLfloat PI = glm::pi<GLfloat>();
const GLfloat HALFPI = PI * 0.5f;
const GLfloat TWOPI = PI * 2.0f;
const GLfloat TODEGREES = PI / 180.0f;
const GLfloat TORADIANS = 180.0f / PI;
const glm::mat2 I2 = glm::mat2(1.0f);
const glm::mat3 I3 = glm::mat3(1.0f);
const glm::mat4 I4 = glm::mat4(1.0f);
const GLfloat FLOAT_MAX = std::numeric_limits<GLfloat>::max();
const GLfloat FLOAT_MIN = std::numeric_limits<GLfloat>::min();

// Forward declartions
namespace geom2d
{
	class Rect;
}
namespace geom3d
{
	class Rect;
}

class MathHelp
{
public:
	static glm::mat4 rotateX(GLfloat radians);
	static glm::mat4 rotateY(GLfloat radians);
	static glm::mat4 rotateZ(GLfloat radians);
	static glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z);
	static glm::mat4 translate(glm::vec3 t);
	static glm::mat4 scale(GLfloat s);
	static glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z);
	static glm::mat4 scale(glm::vec3 s);

	static glm::mat3 MathHelp::skewSym(glm::vec3 a);
	static glm::mat3 rotateVectorToVector(glm::vec3 a, glm::vec3 b);

	static glm::quat Vec4ToQuat(glm::vec4 q);
	static glm::vec4 QuatToVec4(glm::quat q);

	// We define the 2d cross product as the length of the 3d
	static GLfloat cross(glm::vec2 a, glm::vec2 b);

	// Makes a unit vector from radians
	static glm::vec2 slope(GLfloat theta);

	// Compute 2d perp vector (-y, x)
	static glm::vec2 perp(glm::vec2 a);

	static GLfloat sqrDistance(glm::vec3 a, glm::vec3 b);

	static glm::vec2 projAToB(glm::vec2 a, glm::vec2 b);
	static glm::vec3 projAToB(glm::vec3 a, glm::vec3 b);

	// Triangles
	static GLfloat triangleAreaSigned(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	static GLfloat triangleArea(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

	// Computes bary centric coordinates of point p in triangle a,b,c
	static glm::vec3 baryCentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);
	static glm::vec3 baryCentric(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

	// Computes ray given screen position, width, height, fov, and near plane
	//static geom::Ray computeEyeRay(glm::vec2 pos, UINT width, UINT height, GLfloat fov, GLfloat nearZ);

	// Computes intersection point on triangle and then bary centric coordinate, returns vec4(u, v, w, intersectionDepth)
	// If failed, returns vec4(0, 0, 0, max)
	//static glm::vec4 triangleRayIntersection(geom::Ray ray, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 n);

	// Simple lerp between start and end given t [0, 1]
	template<typename T>
	static T lerp(T start, T end, GLfloat t) { return start + t * (end - start); }
	// Samples a function of vectors with linear interpolation
	static glm::vec3 lerp(std::vector<std::tuple<GLfloat, glm::vec3>> func, GLfloat val);

	// Catmull rom interpolation between p1 and p2 given t [0, 1]
	static glm::vec3 catmullRom(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, GLfloat t);

	// Calculates the centroid of a polygon
	static glm::vec2 calcCentroid(glm::vec2* vertices, UINT count);
	static glm::vec3 calcCentroid(glm::vec3* vertices, UINT count);

	// Calculates a bounding rectangle around the point set
	static geom2d::Rect get2dBounds(glm::vec2* vertices, UINT count);
	static geom3d::Rect get3dBounds(glm::vec3* vertices, UINT count);

	static GLfloat trace(glm::mat2 mat) { return mat[0][0] + mat[1][1]; }

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	static glm::mat<C, R, T, Q> outer(glm::vec<C, T, Q> a, glm::vec<R, T, Q> b)
	{
		glm::mat<C, R, T, Q> results;
		for (glm::length_t r = 0; r < R; r++)
		{
			for (glm::length_t c = 0; c < C; c++)
			{
				results[c][r] = a[r] * b[c];
			}
		}
		return results;
	}

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	static void printMat(glm::mat<C, R, T, Q> m)
	{
		for (glm::length_t r = 0; r < R; r++)
		{
			for (glm::length_t c = 0; c < C - 1; c++)
			{
				printf("%f, ", m[c][r]);
			}
			printf("%f\n", m[C - 1][r]);
		}
	}

	template<glm::length_t N, typename T, glm::qualifier Q>
	static void printVec(glm::vec<N, T, Q> vec)
	{
		for (glm::length_t n = 0; n < N - 1; n++)
		{
			printf("%f, ", vec[n]);
		}
		printf("%f\n", vec[N - 1]);
	}

	template<typename T>
	static T clamp(T val, T min, T max)
	{
		if (val > max)
			return max;
		else if (val < min)
			return min;
		else
			return val;
	}

	static void setData(glm::mat2x2& m, GLfloat m00, GLfloat m01, GLfloat m10, GLfloat m11);

#ifdef USEEIGEN
	// Singular value decomp source = u * (s * Identity) * v^T
	static void svd(glm::mat2x2 source, glm::mat2x2* u, glm::vec2* s, glm::mat2x2* v);
	// Polar decomp but only returns the rotational
	static void pd(glm::mat2x2 source, glm::mat2x2* r);
#endif

	// Only for square matrices
	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	static glm::mat<C, R, T, Q> diagProduct(glm::mat<C, R, T, Q> m, glm::vec<C, T, Q> vec)
	{
		glm::mat<C, R, T, Q> results = m;
		for (glm::length_t i = 0; i < C; i++)
		{
			results[i][i] *= vec[i];
		}
		return results;
	}
	// Only for square matrices
	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	static glm::mat<C, R, T, Q> diagSum(glm::mat<C, R, T, Q> m, glm::vec<C, T, Q> vec)
	{
		glm::mat<C, R, T, Q> results = m;
		for (glm::length_t i = 0; i < C; i++)
		{
			results[i][i] += vec[i];
		}
		return results;
	}
	// Only for square matrices
	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	static glm::mat<C, R, T, Q> diagSum(glm::mat<C, R, T, Q> m, GLfloat a)
	{
		glm::mat<C, R, T, Q> results = m;
		for (glm::length_t i = 0; i < C; i++)
		{
			results[i][i] += a;
		}
		return results;
	}

	// Triangle vs Point
	static bool intersectTrianglePoint(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 pt);
	// Segment vs Segment
	static bool intersectSegmentSegment(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2, glm::vec2& intersectionPt, bool inclusive = true);
	// Segment vs Line (give two points on line)
	//static bool intersectSegmentLine(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2, glm::vec2& intersectionPt, bool inclusive = true);
};