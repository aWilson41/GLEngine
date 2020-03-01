#pragma once
#include "AbstractMapper.h"
#include "Types.h"
#include <memory>

class PhongMaterial;
class PolyData;
class ShaderProgram;

// Does rendering and pre rendering operations (mapping input to visual representation)
// Generalized class, makes no assumptions about data provided too it at the cost of some optimization
class PolyDataMapper : public AbstractMapper
{
public:
	PolyDataMapper();
	~PolyDataMapper();

public:
	std::shared_ptr<PolyData> getInput() const { return polyData; }
	std::shared_ptr<ShaderProgram> getShaderProgram() const { return shaderProgram; }
	std::shared_ptr<PhongMaterial> getMaterial() const { return material; }
	glm::mat4 getModelMatrix() const { return model; }
	CellType getPolyRepresentation() const { return representation; }
	GLfloat getPointSize() const { return pointSize; }
	GLfloat getLineWidth() const { return lineWidth; }

	void setInput(std::shared_ptr<PolyData> input) { polyData = input; }
	void setMaterial(std::shared_ptr<PhongMaterial> material) { this->material = material; }
	void setModelMatrix(glm::mat4 model) { this->model = model; }
	void setPolyRepresentation(CellType representation) { this->representation = representation; }
	void setPointSize(GLfloat pointSize) { this->pointSize = pointSize; }
	void setLineWidth(GLfloat lineWidth) { this->lineWidth = lineWidth; }
	void setUseNormals(bool val) { useNormals = val; }
	void setUseTexCoords(bool val) { useTexCoords = val; }
	void setUseScalars(bool val) { useScalars = val; }
	void setUseIndex(bool val) { useIndex = val; }

	void update() override;

	bool useShader(std::string shaderGroup) override;
	void draw(Renderer* ren) const override;

protected:
	// Updates internal info about the data to be processed
	void updateInfo();
	void updateBuffer();

protected:
	std::shared_ptr<PolyData> polyData = nullptr;
	std::shared_ptr<PhongMaterial> material = nullptr;
	glm::mat4 model = glm::mat4(1.0f);

	GLuint vboID = -1;
	GLint vboSize = -1;
	GLuint iboID = -1;
	GLint iboSize = -1;
	GLuint vaoID = -1;

	CellType representation = CellType::TRIANGLE;
	GLfloat pointSize = 4.0f;
	GLfloat lineWidth = 1.0f;

	std::shared_ptr<ShaderProgram> shaderProgram = nullptr;

	// Use normals, scalars, and indices when available
	// User set values
	bool useNormals = true;
	bool useTexCoords = false;
	bool useScalars = true;
	bool useIndex = true;

	// Internally determined
	bool hasNormals = false;
	bool hasTexCoords = false;
	bool hasScalars = false;
	bool hasIndices = false;
};