#include "PrimitivesMapper.h"
#include "AppendPolyData.h"
#include "Camera.h"
#include "CircleSource.h"
#include "PhongMaterial.h"
#include "PolyData.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shaders.h"
#include "SphereSource.h"
#include "stdNew.h"

std::shared_ptr<PrimitivesMapper> Primitives2d::mapper = std::make_shared<PrimitivesMapper>();
std::shared_ptr<PrimitivesMapper> Primitives3d::mapper = std::make_shared<PrimitivesMapper>();

void Primitives2d::setColor(const glm::vec3& color) { mapper->getMaterial()->setDiffuse(color); }
void Primitives2d::setColor(const GLfloat r, const GLfloat g, const GLfloat b) { mapper->getMaterial()->setDiffuse(r, g, b); }

void Primitives3d::setColor(const glm::vec3& color) { mapper->getMaterial()->setDiffuse(color); }
void Primitives3d::setColor(const GLfloat r, const GLfloat g, const GLfloat b) { mapper->getMaterial()->setDiffuse(r, g, b); }

PrimitivesMapper::PrimitivesMapper()
{
	inputPolyData = std::make_shared<PolyData>();
	material = std::make_shared<PhongMaterial>();
	material->setDiffuse(1.0f, 1.0f, 1.0f);
}

PrimitivesMapper::~PrimitivesMapper()
{
	glUseProgram(0);
	if (vaoID != -1)
		glDeleteVertexArrays(1, &vaoID);
	if (vboID != -1)
		glDeleteBuffers(1, &vboID);
	if (iboID != -1)
		glDeleteBuffers(1, &iboID);
}

void PrimitivesMapper::addCircle(const glm::vec2& pos, const GLfloat r, const UINT div)
{
	stdNew<CircleSource> source;
	source->setOrigin(pos.x, pos.y, z);
	source->setRadius(r);
	source->setDivisions(div);
	source->update();
	stdNew<AppendPolyData> appender;
	appender->setInput1(inputPolyData);
	appender->setInput2(source->getOutput());
	appender->update();
	inputPolyData = appender->getOutput();
	update();
}
void PrimitivesMapper::addSphere(const glm::vec3& pos, const GLfloat r, const UINT divTheta, const UINT divPhi)
{
	stdNew<SphereSource> source;
	source->setOrigin(pos);
	source->setRadius(r);
	source->setDivisions(divTheta, divPhi);
	source->update();
	stdNew<AppendPolyData> appender;
	appender->setInput1(inputPolyData);
	appender->setInput2(source->getOutput());
	appender->update();
	inputPolyData = appender->getOutput();
	update();
}
void PrimitivesMapper::addLine(const glm::vec3& p1, const glm::vec3& p2)
{
	stdNew<PolyData> polyData;
	polyData->allocateVertexData(2);
	polyData->allocateIndexData(2, CellType::LINE);
	glm::vec3* vertexData = reinterpret_cast<glm::vec3*>(polyData->getVertexData());
	vertexData[0] = p1;
	vertexData[1] = p2;
	polyData->getIndexData()[0] = 0;
	polyData->getIndexData()[1] = 1;
	stdNew<AppendPolyData> appender;
	appender->setInput1(inputPolyData);
	appender->setInput2(polyData);
	appender->update();
	inputPolyData = appender->getOutput();
	update();
}
void PrimitivesMapper::addArrow(const glm::vec2& p1, const glm::vec2& p2, const GLfloat headSize, const GLfloat headRatio)
{
	stdNew<PolyData> polyData;
	polyData->allocateVertexData(4);
	polyData->allocateIndexData(8, CellType::LINE);

	glm::vec3* vertexData = reinterpret_cast<glm::vec3*>(polyData->getVertexData());
	vertexData[0] = glm::vec3(p1, z);
	vertexData[1] = glm::vec3(p2, z);
	const glm::vec2 diff = p2 - p1;
	const GLfloat length = glm::length(diff);
	const glm::vec2 tan = MathHelp::perp(diff / length) * headSize;
	vertexData[2] = glm::vec3(MathHelp::lerp(p1, p2, headRatio) + tan, z);
	vertexData[3] = glm::vec3(MathHelp::lerp(p1, p2, headRatio) - tan, z);

	UINT* indexData = polyData->getIndexData();
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 1;
	indexData[3] = 2;
	indexData[4] = 1;
	indexData[5] = 3;
	indexData[6] = 2;
	indexData[7] = 3;

	stdNew<AppendPolyData> appender;
	appender->setInput1(inputPolyData);
	appender->setInput2(polyData);
	appender->update();
	inputPolyData = appender->getOutput();
	update();
}

void PrimitivesMapper::clear()
{
	inputPolyData = std::make_shared<PolyData>();
}

void PrimitivesMapper::update()
{
	// Poly data must have vertex data to be visually mapped
	if (inputPolyData->getVertexData() == nullptr)
		return;

	// Determine size of gpu mem to allocate
	const GLuint numPts = inputPolyData->getVertexCount();
	vboSize = sizeof(GLfloat) * 3 * numPts; // Position

	// If the vbo haven't been created yet
	if (vboID == -1)
	{
		// Generate the vao
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		// Gen and allocate space for vbo
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_DYNAMIC_DRAW);
	}
	// If it has already been created
	else
	{
		// Get the current vbo size
		GLint bufferSize;
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

		// If the size has changed reallocate
		if (vboSize != bufferSize)
			glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_DYNAMIC_DRAW);
	}
	// If it hasn't been created yet
	iboSize = sizeof(GLuint) * inputPolyData->getIndexCount();
	if (iboID == -1)
	{
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, inputPolyData->getIndexData(), GL_DYNAMIC_DRAW);
	}
	// If it was already created
	else
	{
		// Get the current ibo size
		GLint bufferSize;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

		// If the size has changed reallocate
		if (iboSize != bufferSize)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, NULL, GL_DYNAMIC_DRAW);
	}
	updateBuffer();
}
void PrimitivesMapper::updateBuffer()
{
	glBindVertexArray(vaoID);

	if (vboID != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		// Load positional data
		GLint size = sizeof(GLfloat) * 3 * inputPolyData->getVertexCount();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, inputPolyData->getVertexData());
		// Set it's location and access scheme in vao
		GLuint posAttribLocation = 0;// glGetAttribLocation(shaderID, "inPos");
		glEnableVertexAttribArray(posAttribLocation);
		glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)0);
	}
	// Update index buffer if it has one and buffer has been created
	if (iboID != -1)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * inputPolyData->getIndexCount(), inputPolyData->getIndexData());
	}

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool PrimitivesMapper::use(const std::string& shaderGroup)
{
	if (inputPolyData == nullptr || vaoID == -1)
		return false;

	if (objectProperties->isOutOfDate())
		shaderProgram = Shaders::getShader(shaderGroup, "PrimitivesMapper", &properties);

	if (shaderProgram == nullptr)
		return false;

	glUseProgram(shaderProgram->getProgramID());
	return true;
}

void PrimitivesMapper::draw(std::shared_ptr<Camera> cam, std::shared_ptr<Scene> scene) const
{
	if (inputPolyData == nullptr || vaoID == -1)
		return;

	// Save the polygon mode
	GLint prevPolyMode;
	glGetIntegerv(GL_POLYGON_MODE, &prevPolyMode);

	// Set object uniforms
	const GLuint shaderProgramId = shaderProgram->getProgramID();
	glm::mat4 viewProj = cam->proj * cam->view;
	const GLuint mvpMatrixLocation = glGetUniformLocation(shaderProgramId, "mvp_matrix");
	if (mvpMatrixLocation != -1)
		glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &viewProj[0][0]);
	const GLuint viewDirLocation = glGetUniformLocation(shaderProgramId, "viewDir");
	if (viewDirLocation != -1)
		glUniform3fv(viewDirLocation, 1, &cam->getLookDir()[0]);
	const GLuint lineThicknessLocation = glGetUniformLocation(shaderProgramId, "lineThickness");
	if (lineThicknessLocation != -1)
		glUniform1f(lineThicknessLocation, lineWidth);
	glm::vec4 diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	if (material != nullptr)
	{
		diffuse = material->getDiffuse();
		specular = material->getSpecular();
		ambient = material->getAmbient();
	}
	const GLuint diffuseLocation = glGetUniformLocation(shaderProgramId, "mat.diffuseColor");
	if (diffuseLocation != -1)
		glUniform4fv(diffuseLocation, 1, &diffuse[0]);
	const GLuint specularLocation = glGetUniformLocation(shaderProgramId, "mat.specularColor");
	if (specularLocation != -1)
		glUniform3fv(specularLocation, 1, &specular[0]);
	const GLuint ambientLocation = glGetUniformLocation(shaderProgramId, "mat.ambientColor");
	if (ambientLocation != -1)
		glUniform3fv(ambientLocation, 1, &ambient[0]);
	// Set the scene uniforms
	const GLuint lightDirLocation = glGetUniformLocation(shaderProgramId, "lightDir");
	if (lightDirLocation != -1)
		glUniform3fv(lightDirLocation, 1, &scene->getLightDir()[0]);

	glBindVertexArray(vaoID);
	glDrawElements(GL_LINES, inputPolyData->getIndexCount(), GL_UNSIGNED_INT, (void*)(uintptr_t)0);
	glBindVertexArray(0);

	// Restore poly mode
	glPolygonMode(GL_FRONT_AND_BACK, prevPolyMode);
}