#include "GlyphPolyDataMapper.h"
#include "Camera.h"
#include "PhongMaterial.h"
#include "PolyData.h"
#include "Scene.h"
#include "Shaders.h"

// Maps cell type to mode
static std::map<CellType, GLenum> mode = {
	{ CellType::POINT, GL_POINTS },
	{ CellType::LINE, GL_LINES },
	{ CellType::TRIANGLE, GL_TRIANGLES },
	{ CellType::QUAD, GL_QUADS } };

GlyphPolyDataMapper::GlyphPolyDataMapper()
{
	// Don't inherit parent properties
	objectProperties->clear();
	objectProperties->addProperty("Use_Scalars", false);
	objectProperties->addProperty("Use_Indices", false);
}

GlyphPolyDataMapper::~GlyphPolyDataMapper()
{
	if (offsetData != nullptr)
		delete[] offsetData;
}

void GlyphPolyDataMapper::update()
{
	const GLfloat* vertexData = polyData->getVertexData();
	const GLfloat* normalData = polyData->getNormalData();
	const GLuint* indexData = polyData->getIndexData();

	// Poly data must have vertex data to be visually mapped
	if (vertexData == nullptr || normalData == nullptr)
		return;

	// Set the shader to use
	updateInfo();

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
	// If we have index data
	if (indexData != nullptr && useIndex)
	{
		// If it hasn't been created yet
		iboSize = sizeof(GLuint) * polyData->getIndexCount();
		if (iboID == -1)
		{
			glGenBuffers(1, &iboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, polyData->getIndexData(), GL_DYNAMIC_DRAW);
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
	}
	updateBuffer();

	representation = std::min(std::max(representation, CellType::POINT), polyData->getCellType());
}
void GlyphPolyDataMapper::updateInfo()
{
	hasNormals = (polyData->getNormalData() != nullptr && useNormals);
	hasScalars = (colorData != nullptr && useScalars);
	hasIndices = (polyData->getIndexData() != nullptr && useIndex);
	hasTexCoords = useTexCoords = false;

	objectProperties->setProperty("Use_Scalars", hasScalars);
	objectProperties->setProperty("Use_Indices", hasIndices);

	// Determine size of gpu mem to allocate we assume it has normals and offsets
	const GLuint numPts = polyData->getVertexCount();
	vboSize = sizeof(GLfloat) * (6 * numPts + instanceCount * 3); // Position and normals + offsets
	if (hasScalars)
		vboSize += sizeof(GLfloat) * 3 * instanceCount;
}
void GlyphPolyDataMapper::updateBuffer()
{
	glBindVertexArray(vaoID);

	const GLfloat* vertexData = polyData->getVertexData();
	const GLfloat* normalData = polyData->getNormalData();
	const GLfloat* scalarData = colorData;
	const GLint numPts = polyData->getVertexCount();

	if (vboID != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		// Load positional data
		GLint size = sizeof(GLfloat) * 3 * numPts;
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexData);
		// Set it's location and access scheme in vao
		GLuint posAttribLocation = 0;// glGetAttribLocation(shaderID, "inPos");
		glEnableVertexAttribArray(posAttribLocation);
		glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)0);

		GLint offset = size;

		size = sizeof(GLfloat) * 3 * numPts;
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, normalData);
		// Set it's location and access scheme in vao
		GLuint normalAttribLocation = 1;// glGetAttribLocation(shaderID, "inNormal");
		glEnableVertexAttribArray(normalAttribLocation);
		glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)offset);

		offset += size;

		size = sizeof(GLfloat) * 3 * instanceCount;
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, offsetData);
		// Set it's location and access scheme in vao
		GLuint offsetAttribLocation = 2;// glGetAttribLocation(shaderID, "inOffset");
		glEnableVertexAttribArray(offsetAttribLocation);
		glVertexAttribPointer(offsetAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)offset);
		glVertexAttribDivisor(offsetAttribLocation, 1);

		offset += size;

		if (hasScalars)
		{
			size = sizeof(GLfloat) * 3 * instanceCount;
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, scalarData);
			// Set it's location and access scheme in vao
			GLuint colorAttribLocation = 3;// glGetAttribLocation(shaderID, "inColor");
			glEnableVertexAttribArray(colorAttribLocation);
			glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)offset);
			glVertexAttribDivisor(colorAttribLocation, 1);
		}
	}
	// Update index buffer if it has one and buffer has been created
	if (iboID != -1)
	{
		if (hasIndices)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * polyData->getIndexCount(), polyData->getIndexData());
		}
	}

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool GlyphPolyDataMapper::use(const std::string& shaderGroup)
{
	if (polyData == nullptr || vaoID == -1)
		return false;

	if (objectProperties->isOutOfDate())
		shaderProgram = Shaders::getShader(shaderGroup, "GlyphPolyDataMapper", &properties);

	if (shaderProgram == nullptr)
		return false;

	glUseProgram(shaderProgram->getProgramID());
	return true;
}

void GlyphPolyDataMapper::draw(std::shared_ptr<Camera> cam, std::shared_ptr<Scene> scene) const
{
	if (polyData == nullptr || vaoID == -1)
		return;

	// Save the polygon mode
	GLint polyMode;
	glGetIntegerv(GL_POLYGON_MODE, &polyMode);

	// Set the polygon mode needed
	if (representation == CellType::TRIANGLE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (representation == CellType::LINE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (representation == CellType::POINT)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(pointSize);
	}

	// Set the uniforms
	const GLuint shaderProgramId = shaderProgram->getProgramID();
	glm::mat4 mvp = cam->proj * cam->view * model;
	const GLuint mvpMatrixLocation = glGetUniformLocation(shaderProgramId, "mvp_matrix");
	if (mvpMatrixLocation != -1)
		glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mvp[0][0]);
	glm::vec4 diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	if (material != nullptr)
	{
		diffuse = material->getDiffuse();
		ambient = material->getAmbient();
	}
	const GLuint diffuseLocation = glGetUniformLocation(shaderProgramId, "mat.diffuseColor");
	if (diffuseLocation != -1)
		glUniform4fv(diffuseLocation, 1, &diffuse[0]);
	const GLuint ambientLocation = glGetUniformLocation(shaderProgramId, "mat.ambientColor");
	if (ambientLocation != -1)
		glUniform3fv(ambientLocation, 1, &ambient[0]);
	// Set the scene uniforms
	const GLuint lightDirLocation = glGetUniformLocation(shaderProgramId, "lightDir");
	if (lightDirLocation != -1)
		glUniform3fv(lightDirLocation, 1, &scene->getLightDir()[0]);

	glBindVertexArray(vaoID);
	const CellType cellType = polyData->getCellType();
	if (polyData->getIndexData() != nullptr && useIndex)
		glDrawElementsInstanced(mode[cellType], polyData->getIndexCount(), GL_UNSIGNED_INT, (void*)(uintptr_t)0, instanceCount);
	else
		glDrawArraysInstanced(mode[cellType], 0, static_cast<GLsizei>(polyData->getVertexCount()), instanceCount);
	glBindVertexArray(0);

	// Set the poly mode back to what it was
	glPolygonMode(GL_FRONT_AND_BACK, polyMode);
}