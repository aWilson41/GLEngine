#include "MarchingSquares.h"
#include "ImageData.h"
#include "ParallelFor.h"
#include "PolyData.h"
#include <unordered_map>

enum MC_CASE
{
	NONE = 0,
	BOT_LEFT = 1,
	BOT_RIGHT = 2,
	TOP_RIGHT = 4,
	TOP_LEFT = 8
};
inline MC_CASE operator |(MC_CASE a, MC_CASE b) { return static_cast<MC_CASE>(static_cast<int>(a) | static_cast<int>(b)); }
inline MC_CASE& operator |=(MC_CASE& a, MC_CASE b) {return a = a | b; }

// Number of vertices per MC_CASE
std::unordered_map<MC_CASE, int> caseToNumVertices = {
	{ static_cast<MC_CASE>(0), 0 },
	{ static_cast<MC_CASE>(1), 2 },
	{ static_cast<MC_CASE>(2), 2 },
	{ static_cast<MC_CASE>(3), 2 },
	{ static_cast<MC_CASE>(4), 2 },
	{ static_cast<MC_CASE>(5), 4 },
	{ static_cast<MC_CASE>(6), 2 },
	{ static_cast<MC_CASE>(7), 2 },
	{ static_cast<MC_CASE>(8), 2 },
	{ static_cast<MC_CASE>(9), 2 },
	{ static_cast<MC_CASE>(10), 4 },
	{ static_cast<MC_CASE>(11), 2 },
	{ static_cast<MC_CASE>(12), 2 },
	{ static_cast<MC_CASE>(13), 2 },
	{ static_cast<MC_CASE>(14), 2 },
	{ static_cast<MC_CASE>(15), 0 } };

static UINT calcIndex(UINT x, UINT y, UINT* dim) { return x + y * dim[0]; }

MarchingSquares::MarchingSquares()
{
	outputData = std::make_shared<PolyData>();

	// Load the compute shader
	/*std::string shaderDir = "Shaders/Common/";
	std::vector<std::string> shaderPaths;
	shaderPaths.push_back(shaderDir + "marchingSquares.glsl");
	shaderProgram = Shaders::loadComputeShader("main", shaderPaths);*/
}

void MarchingSquares::update()
{
	outputData->clear();

	if (inputData->getScalarType() != ScalarType::FLOAT_T)
	{
		printf("MarchingSquares only supports float type");
		return;
	}

	// Using a 2x2 kernel identify areas with differing values
	// Identify the case types and compute weights
	UINT* dim = inputData->getDimensions();
	float* imgPtr = inputData->getData<float>();
	const UINT numPx = dim[0] * dim[1];
	const UINT numGroups = (dim[0] - 1) * (dim[1] - 1);
	std::vector<MC_CASE> caseTypes(numGroups);
	std::fill_n(caseTypes.begin(), numGroups, MC_CASE::NONE);
	tbb::atomic<UINT> vertexCount(0);
	parallelFor(dim[1] - 1, [&](const UINT& y)
		{
			UINT i = y * (dim[0] - 1);
			UINT j = y * dim[0];
			UINT innerVertexCount = 0;
			for (UINT x = 0; x < dim[0] - 1; x++, i++, j++)
			{
				const float vals[4] = {
					imgPtr[j + 1],
					imgPtr[j + dim[0] + 1],
					imgPtr[j + dim[0]],
					imgPtr[j] };

				if (vals[3] > isoValue)
					caseTypes[i] |= BOT_LEFT;
				if (vals[0] > isoValue)
					caseTypes[i] |= BOT_RIGHT;
				if (vals[1] > isoValue)
					caseTypes[i] |= TOP_RIGHT;
				if (vals[2] > isoValue)
					caseTypes[i] |= TOP_LEFT;
				innerVertexCount += caseToNumVertices[caseTypes[i]];
			}
			if (innerVertexCount != 0)
				vertexCount += innerVertexCount;
		});

	// Allocate the polydata
	outputData->allocateVertexData(vertexCount);
	outputData->setCellType(CellType::LINE);
	glm::vec3* vertices = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	const glm::vec2 spacing = glm::vec2(inputData->getSpacing()[0], inputData->getSpacing()[1]);
	const glm::vec2 origin = glm::vec2(inputData->getOrigin()[0], inputData->getOrigin()[1]);
	const glm::vec2 size = glm::vec2(dim[0], dim[1]) * spacing;
	const glm::vec2 shift = origin + spacing * 0.5f;
	UINT polyVertexIter = 0;
	UINT i = 0;
	UINT j = 0;
	for (UINT y = 0; y < dim[1] - 1; y++, j++)
	{
		for (UINT x = 0; x < dim[0] - 1; x++, i++, j++)
		{
			const float vals[4] = {
				imgPtr[j + 1],
				imgPtr[j + dim[0] + 1],
				imgPtr[j + dim[0]],
				imgPtr[j] };
			const glm::vec2 pts[4] = {
				glm::vec2(x + 1, y) * spacing + shift,
				glm::vec2(x + 1, y + 1) * spacing + shift,
				glm::vec2(x, y + 1) * spacing + shift,
				glm::vec2(x, y) * spacing + shift
			};
			MC_CASE caseType = caseTypes[i];

			// Only bottom left
			if (caseType == 1)
			{
				// Compute bottom edge
				const float t_x = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x * spacing.x, pts[3].y, 0.0f);
				// Compute left edge
				const float t_y = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y * spacing.y, 0.0f);
			}
			else if (caseType == 2)
			{
				// Compute right edge
				const float t_y = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y * spacing.y, 0.0f);
				// Compute bottom edge
				const float t_x = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x * spacing.x, pts[3].y, 0.0f);
			}
			else if (caseType == 3)
			{
				// Compute right edge
				const float t_y2 = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y2 * spacing.y, 0.0f);
				// Compute left edge
				const float t_y1 = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y1 * spacing.y, 0.0f);
			}
			else if (caseType == 4)
			{
				// Compute top edge
				const float t_x = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x * spacing.x, pts[1].y, 0.0f);
				// Compute right edge
				const float t_y = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y * spacing.y, 0.0f);
			}
			else if (caseType == 5)
			{
				// Compute right edge
				const float t_y1 = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y1 * spacing.y, 0.0f);
				// Compute bottom edge
				const float t_x1 = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x1 * spacing.x, pts[3].y, 0.0f);
				
				// Compute left edge
				const float t_y2 = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y2 * spacing.y, 0.0f);
				// Compute top edge
				const float t_x2 = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x2 * spacing.x, pts[1].y, 0.0f);
			}
			else if (caseType == 6)
			{
				// Compute top edge
				const float t_x1 = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x1 * spacing.x, pts[1].y, 0.0f);
				// Compute bottom edge
				const float t_x2 = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x2 * spacing.x, pts[3].y, 0.0f);
			}
			else if (caseType == 7)
			{
				// Compute top edge
				const float t_x = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x * spacing.x, pts[1].y, 0.0f);
				// Compute left edge
				const float t_y = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y * spacing.y, 0.0f);
			}
			else if (caseType == 8)
			{
				// Compute left edge
				const float t_y = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y * spacing.y, 0.0f);
				// Compute top edge
				const float t_x = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x * spacing.x, pts[1].y, 0.0f);
			}
			else if (caseType == 9)
			{
				// Compute bottom edge
				const float t_x1 = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x1 * spacing.x, pts[3].y, 0.0f);
				// Compute top edge
				const float t_x2 = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x2 * spacing.x, pts[1].y, 0.0f);
			}
			else if (caseType == 10)
			{
				// Compute bottom edge
				const float t_x1 = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x1 * spacing.x, pts[3].y, 0.0f);
				// Compute left edge
				const float t_y1 = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y1 * spacing.y, 0.0f);

				// Compute top edge
				const float t_x2 = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x2 * spacing.x, pts[1].y, 0.0f);
				// Compute right edge
				const float t_y2 = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y2 * spacing.y, 0.0f);
			}
			else if (caseType == 11)
			{
				// Compute right edge
				const float t_y = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y * spacing.y, 0.0f);
				// Compute top edge
				const float t_x2 = (vals[1] - isoValue) / (vals[1] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[1].x - t_x2 * spacing.x, pts[1].y, 0.0f);
			}
			else if (caseType == 12)
			{
				// Compute left edge
				const float t_y1 = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y1 * spacing.y, 0.0f);
				// Compute right edge
				const float t_y2 = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y2 * spacing.y, 0.0f);
			}
			else if (caseType == 13)
			{
				// Compute bottom edge
				const float t_x = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x * spacing.x, pts[3].y, 0.0f);
				// Compute right edge
				const float t_y = (vals[0] - isoValue) / (vals[0] - vals[1]);
				vertices[polyVertexIter++] = glm::vec3(pts[0].x, pts[0].y + t_y * spacing.y, 0.0f);
			}
			else if (caseType == 14)
			{
				// Compute left edge
				const float t_y = (vals[3] - isoValue) / (vals[3] - vals[2]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x, pts[3].y + t_y * spacing.y, 0.0f);
				// Compute bottom edge
				const float t_x = (vals[3] - isoValue) / (vals[3] - vals[0]);
				vertices[polyVertexIter++] = glm::vec3(pts[3].x + t_x * spacing.x, pts[3].y, 0.0f);
			}
		}
	}

	//GLuint shaderID = shaderProgram->getProgramID();
	//glUseProgram(shaderID);

	//// Create opengl texture from image

	//// Create a polygon

	//// Bind the texture
	//glBindImageTexture(0, colorTexID, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

	//static GLint workGroupSize[3] = { -1, -1, -1 };
	//glGetProgramiv(shaderID, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
	//const glm::ivec2 fboDim = getFramebufferDim();
	//int numGroupsX = nextPowerOfTwo(fboDim.x) / workGroupSize[0];
	//int numGroupsY = nextPowerOfTwo(fboDim.y) / workGroupSize[1];
	//glDispatchCompute(numGroupsX, numGroupsY, 1);

	//// Block until the image is written
	//glBindImageTexture(0, 0, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	//glUseProgram(0);
}