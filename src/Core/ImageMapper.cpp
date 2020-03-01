#include "ImageMapper.h"
#include "Camera.h"
#include "ImageData.h"
#include "PlaneSource.h"
#include "PolyData.h"
#include "Renderer.h"
#include "Shaders.h"

ImageMapper::ImageMapper()
{
	planeSource = std::make_shared<PlaneSource>();
	planeSource->update();

	objectProperties->clear();
	objectProperties->addProperty("FormatGrayscale", false);
	objectProperties->addProperty("FormatRGB", false);
}

ImageMapper::~ImageMapper()
{
	glUseProgram(0);
	if (vaoID != -1)
		glDeleteVertexArrays(1, &vaoID);
	if (vboID != -1)
		glDeleteBuffers(1, &vboID);
	if (iboID != -1)
		glDeleteBuffers(1, &iboID);
}

void ImageMapper::update()
{
	// Has no input
	if (imageData == nullptr)
		return;

	const GLuint numComps = imageData->getNumComps();
	if (numComps == 1)
	{
		objectProperties->setProperty("FormatGrayscale", true);
		objectProperties->setProperty("FormatRGB", false);
	}
	else if (numComps == 3)
	{
		objectProperties->setProperty("FormatGrayscale", false);
		objectProperties->setProperty("FormatRGB", true);
	}
	else
		printf("Error: ImageMapper only supports 1 or 3 components.\n");

	// Transform the plane the image goes on to be in the XY plane and be the size of the image
	double* bounds = imageData->getBounds();
	const glm::vec2 size = glm::vec2(static_cast<GLfloat>(bounds[1] - bounds[0]), static_cast<GLfloat>(bounds[3] - bounds[2]));
	imageSizeMat = MathHelp::scale(size.x, size.y, 1.0f) * MathHelp::rotateX(HALFPI);

	// If the vao and vbo haven't been created yet
	if (vaoID == -1)
	{
		// Generate the vao
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		// Gen and allocate space for vbo
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		const GLuint numPts = planeSource->getOutput()->getVertexCount(); // 3coords 2texcoords
		glBufferData(GL_ARRAY_BUFFER, numPts * sizeof(GLfloat) * 5, NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, NULL, GL_DYNAMIC_DRAW);

		updateBuffer();
	}
	else
		// VBO size should be constant so no reallocating needed
		updateBuffer();

	// Setup the texture if it hasn't already been created
	const GLuint* dim = imageData->getDimensions();
	if (texID == -1)
	{
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (numComps == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dim[0], dim[1], 0, GL_RED, GL_UNSIGNED_BYTE, imageData->getData());
		else if (numComps == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim[0], dim[1], 0, GL_RGB, GL_UNSIGNED_BYTE, imageData->getData());

		glUniform1i(glGetUniformLocation(0, "tex"), 0);
	}
	// If it already has update the one in there
	else
	{
		glBindTexture(GL_TEXTURE_2D, texID);
		if (numComps == 1)
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, dim[0], dim[1], GL_RED, GL_UNSIGNED_BYTE, imageData->getData());
		else if (numComps == 3)
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, dim[0], dim[1], GL_RGB, GL_UNSIGNED_BYTE, imageData->getData());
	}
}
void ImageMapper::updateBuffer()
{
	const GLfloat* vertexData = planeSource->getOutput()->getVertexData();
	const GLfloat* texCoordData = planeSource->getOutput()->getTexCoordData();
	const UINT* indexData = planeSource->getOutput()->getIndexData();
	const GLint numPts = planeSource->getOutput()->getVertexCount();

	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	// Load positional data
	const GLint size1 = sizeof(GLfloat) * 3 * numPts;
	glBufferSubData(GL_ARRAY_BUFFER, 0, size1, vertexData);
	// Set it's location and access scheme in vao
	const GLuint posAttribLocation = 0;// glGetAttribLocation(shaderID, "inPos");
	glEnableVertexAttribArray(posAttribLocation);
	glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(uintptr_t)0);

	const GLint size2 = sizeof(GLfloat) * 2 * numPts;
	glBufferSubData(GL_ARRAY_BUFFER, size1, size2, texCoordData);
	// Set it's location and access scheme in vao
	const GLuint texCoordAttribLocation = 2;// glGetAttribLocation(shaderID, "inTexCoord");
	glEnableVertexAttribArray(texCoordAttribLocation);
	glVertexAttribPointer(texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(uintptr_t)size1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * 6, indexData);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool ImageMapper::useShader(std::string shaderGroup)
{
	if (imageData == nullptr || vaoID == -1)
		return false;

	if (objectProperties->isOutOfDate())
		shaderProgram = Shaders::getShader(shaderGroup, "ImageMapper", &properties);

	if (shaderProgram == nullptr)
		return false;

	glUseProgram(shaderProgram->getProgramID());
	return true;
}

void ImageMapper::draw(Renderer* ren) const
{
	if (imageData == nullptr || vaoID == -1)
		return;

	// Save and set the poly mode
	GLint polyMode;
	glGetIntegerv(GL_POLYGON_MODE, &polyMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Object uniforms
	const GLuint shaderProgramId = shaderProgram->getProgramID();
	const glm::mat4 mvp = ren->getCamera()->proj * ren->getCamera()->view * model * imageSizeMat;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "mvp_matrix"), 1, GL_FALSE, &mvp[0][0]);
	// Set the scene uniforms
	const GLuint lightDirLocation = glGetUniformLocation(shaderProgramId, "lightDir");
	if (lightDirLocation != -1)
		glUniform3fv(lightDirLocation, 1, &ren->getLightDir()[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(uintptr_t)0);
	glBindVertexArray(0);

	// Set the poly mode back to what it was
	glPolygonMode(GL_FRONT_AND_BACK, polyMode);
}