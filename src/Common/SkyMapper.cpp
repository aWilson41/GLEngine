#include "SkyMapper.h"
#include "ArHosekSkyModelData_RGB.h"
#include "Camera.h"
#include "PhongMaterial.h"
#include "PolyData.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Shaders.h"

const UINT vertexCount = 8;
static GLfloat vertexData[vertexCount * 3] = {
	-1.0f, -1.0f, 1.0f, // 0: Left, bottom, front
	1.0f, -1.0f, 1.0f, // 1: Right, bottom, front
	1.0f, 1.0f, 1.0f, // 2: Right, top, front
	-1.0f, 1.0f, 1.0f, // 3: Left, top, front
	-1.0f, -1.0f, -1.0f, // 4: Left, bottom, back
	1.0f, -1.0f, -1.0f, // 5: Right, bottom, back
	1.0f, 1.0f, -1.0f, // 6: Right, top, back
	-1.0f, 1.0f, -1.0f }; // 7: Left, top, back

const UINT indexCount = 36;
static int indexData[indexCount] = {
	2, 1, 0, 0, 3, 2, // Front
	5, 6, 7, 7, 4, 5, // Back
	3, 0, 4, 4, 7, 3, // Left
	6, 5, 1, 1, 2, 6, // Top
	6, 2, 3, 3, 7, 6, // Right
	0, 1, 5, 5, 4, 0 }; // Bottom

static GLfloat evalSpline(float* dataset, int start, int stride, GLfloat elevation)
{
	return 1.0f * glm::pow(1.0f - elevation, 5) * dataset[start + 0 * stride] +
		5.0f * glm::pow(1.0f - elevation, 4) * glm::pow(elevation, 1) * dataset[start + 1 * stride] +
		10.0f * glm::pow(1.0f - elevation, 3) * glm::pow(elevation, 2) * dataset[start + 2 * stride] +
		10.0f * glm::pow(1.0f - elevation, 2) * glm::pow(elevation, 3) * dataset[start + 3 * stride] +
		5.0f * glm::pow(1.0f - elevation, 1) * glm::pow(elevation, 4) * dataset[start + 4 * stride] +
		1.0f * glm::pow(elevation, 5) * dataset[start + 5 * stride];
}

static GLfloat eval(float* dataset, int stride, GLfloat turbidity, GLfloat albedo, GLfloat theta)
{
	// splines are functions of elevation^1/3
	const GLfloat elevationK = glm::pow(glm::max(1.0f - theta / HALFPI, 0.0f), 1.0f / 3.0f);

	// table has values for turbidity 1..10
	const int turbidity0 = glm::clamp(static_cast<int>(turbidity), 1, 10);
	const int turbidity1 = glm::min(turbidity0 + 1, 1);
	const GLfloat turbidityK = glm::clamp(turbidity - turbidity0, 0.0f, 1.0f);

	const int datasetA0 = 0;
	const int datasetA1 = stride * 6 * 10;

	const GLfloat a0t0 = evalSpline(dataset, datasetA0 + stride * 6 * (turbidity0 - 1), stride, elevationK);
	const GLfloat a1t0 = evalSpline(dataset, datasetA1 + stride * 6 * (turbidity0 - 1), stride, elevationK);
	const GLfloat a0t1 = evalSpline(dataset, datasetA0 + stride * 6 * (turbidity1 - 1), stride, elevationK);
	const GLfloat a1t1 = evalSpline(dataset, datasetA1 + stride * 6 * (turbidity1 - 1), stride, elevationK);

	return a0t0 * (1.0f - albedo) * (1.0f - turbidityK) + a1t0 * albedo * (1.0f - turbidityK) + a0t1 * (1.0f - albedo) * turbidityK + a1t1 * albedo * turbidityK;
}

glm::vec3 hosek_wilkie(GLfloat theta, GLfloat gamma, GLfloat cosGamma, glm::vec3* params)
{
	const glm::vec3& A = params[0];
	const glm::vec3& B = params[1];
	const glm::vec3& C = params[2];
	const glm::vec3& D = params[3];
	const glm::vec3& E = params[4];
	const glm::vec3& F = params[5];
	const glm::vec3& G = params[6];
	const glm::vec3& H = params[7];
	const glm::vec3& I = params[8];

	const glm::vec3 chi = (1.0f + cosGamma * cosGamma) / glm::pow(H * H + 1.0f - 2.0f * cosGamma * H, glm::vec3(1.5f));
	return (A * glm::exp(B / (theta + 0.01f)) + 1.0f) * (C + D * glm::exp(E * gamma) + F * (cosGamma * cosGamma) + G * chi + I * glm::sqrt(glm::max(theta, 0.0f)));
}

SkyMapper::~SkyMapper()
{
	glUseProgram(0);
	if (vaoID != -1)
		glDeleteVertexArrays(1, &vaoID);
	if (vboID != -1)
		glDeleteBuffers(1, &vboID);
	if (iboID != -1)
		glDeleteBuffers(1, &iboID);
}

void SkyMapper::update()
{
	// Poly data must have vertex data to be visually mapped
	if (vertexData == nullptr)
		return;

	// If the vbo haven't been created yet
	if (vboID == -1)
	{
		// Generate the vao
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		// Generate and fill the vertex buffer
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

		// Genreate and fill the index buffer
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		// Specify the vertex layout
		const GLuint posAttribLocation = 0;
		glEnableVertexAttribArray(posAttribLocation);
		glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// let hor = (sun_dir.x * sun_dir.x + sun_dir.z * sun_dir.z).sqrt();
	const GLfloat lightTheta = glm::acos(glm::clamp(lightDir.y, 0.0f, 1.0f));
	for (UINT i = 0; i < 3; i++)
	{
		params[0][i] = eval(&datasetsRGB[i][0], 9, turbidity, albedo[i], lightTheta);
		params[1][i] = eval(&datasetsRGB[i][1], 9, turbidity, albedo[i], lightTheta);
		params[2][i] = eval(&datasetsRGB[i][2], 9, turbidity, albedo[i], lightTheta);
		params[3][i] = eval(&datasetsRGB[i][3], 9, turbidity, albedo[i], lightTheta);
		params[4][i] = eval(&datasetsRGB[i][4], 9, turbidity, albedo[i], lightTheta);
		params[5][i] = eval(&datasetsRGB[i][5], 9, turbidity, albedo[i], lightTheta);
		params[6][i] = eval(&datasetsRGB[i][6], 9, turbidity, albedo[i], lightTheta);

		params[7][i] = eval(&datasetsRGB[i][8], 9, turbidity, albedo[i], lightTheta);
		params[8][i] = eval(&datasetsRGB[i][7], 9, turbidity, albedo[i], lightTheta);

		// Z value thing
		params[9][i] = eval(datasetsRGBRad[i], 1, turbidity, albedo[i], lightTheta);
	}
	glm::vec3 S = hosek_wilkie(glm::cos(lightTheta), 0.0f, 1.0f, params) * params[9];

	params[9] /= glm::dot(S, glm::vec3(0.2126f, 0.7152f, 0.0722f));
	
	GLfloat sunAmount = std::fmod(lightDir.y / HALFPI, 4.0f);
	if (sunAmount > 2.0f)
		sunAmount = 0.0f;
	if (sunAmount > 1.0f)
		sunAmount = 2.0f - sunAmount;
	else if (sunAmount < -1.0f)
		sunAmount = -2.0f - sunAmount;
	params[9] *= 0.6f + 0.45f * sunAmount;
}

bool SkyMapper::use(const std::string& shaderGroup)
{
	if (objectProperties->isOutOfDate())
	{
		shaderProgram = Shaders::loadVSFSShader("HosekWilki", "Shaders/Common/hosekWilkieVS.glsl", "Shaders/Common/hosekWilkieFS.glsl");
		properties.update();
	}

	if (shaderProgram == nullptr)
		return false;

	glUseProgram(shaderProgram->getProgramID());
	return true;
}

void SkyMapper::draw(std::shared_ptr<Camera> cam, std::shared_ptr<Scene> scene) const
{
	if (vaoID == -1)
		return;

	// Save the polygon mode
	GLint polyMode;
	glGetIntegerv(GL_POLYGON_MODE, &polyMode);

	// Set the polygon mode needed
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthMask(GL_FALSE);

	// Set the uniforms
	const GLuint shaderProgramId = shaderProgram->getProgramID();

	glm::quat orientation;
	glm::vec3 translate, scale, skew;
	glm::vec4 persp;
	glm::decompose(cam->view, scale, orientation, translate, skew, persp);

	const glm::mat4 viewProj = cam->proj * glm::toMat4(orientation);
	const GLuint projMatrixLocation = glGetUniformLocation(shaderProgramId, "projMatrix");
	if (projMatrixLocation != -1)
		glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &viewProj[0][0]);
	// Set the scene uniforms
	const GLuint lightDirLocation = glGetUniformLocation(shaderProgramId, "lightDir");
	if (lightDirLocation != -1)
		glUniform3fv(lightDirLocation, 1, &scene->getLightDir()[0]);
	const GLuint paramsLocation = glGetUniformLocation(shaderProgramId, "params");
	if (paramsLocation != -1)
		glUniform3fv(paramsLocation, 10, &params[0][0]);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(uintptr_t)0);
	glBindVertexArray(0);

	// Set the poly mode back to what it was
	glPolygonMode(GL_FRONT_AND_BACK, polyMode);

	glDepthMask(GL_TRUE);
}