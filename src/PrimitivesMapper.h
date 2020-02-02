#pragma once
#include "AbstractMapper.h"
#include "Singleton.h"
#include <memory>

class PhongMaterial;
class PolyData;
class ShaderProgram;

// A mapper for rendering some basic 2d primitives
// Intended for debug purposes but could see other uses
class PrimitivesMapper : public AbstractMapper
{
public:
	PrimitivesMapper();
	~PrimitivesMapper();

public:
	ShaderProgram* getShaderProgram() const { return shaderProgram; }
	std::shared_ptr<PhongMaterial> getMaterial() const { return material; }
	GLfloat getLineWidth() const { return lineWidth; }
	GLuint getShaderProgramID() const override;
	std::string getMapperName() const override { return "PrimitivesMapper"; }

	void setMaterial(std::shared_ptr<PhongMaterial> material) { this->material = material; }
	void setLineWidth(GLfloat lineWidth) { this->lineWidth = lineWidth; }
	void setZ(GLfloat z) { this->z = z; }

	void addCircle(glm::vec2 pos, GLfloat r, UINT div);
	void addLine(glm::vec2 p1, glm::vec2 p2);
	void addArrow(glm::vec2 p1, glm::vec2 p2, GLfloat headSize = 1.0f);

	// Removes all geometry
	void clear();

	void update();

	bool useShader(std::string shaderGroup) override;
	void draw(Renderer* ren) const override;

protected:
	void updateBuffer();

protected:
	std::shared_ptr<PolyData> inputPolyData = nullptr;
	std::shared_ptr<PhongMaterial> material = nullptr;

	GLuint vboID = -1;
	GLint vboSize = -1;
	GLuint iboID = -1;
	GLint iboSize = -1;
	GLuint vaoID = -1;

	GLfloat lineWidth = 2.0f;
	GLfloat z = -1.0f;

	ShaderProgram* shaderProgram = nullptr;
};

// Class for debug annotation rendering, static mapper must be added to your renderer
class Primitives2d : public Singleton<Primitives2d>
{
public:
	static std::shared_ptr<PrimitivesMapper> getMapper() { return mapper; }

	static void setZ(GLfloat z) { mapper->setZ(z); }

	static void addCircle(glm::vec2 pos, GLfloat r, UINT div) { mapper->addCircle(pos, r, div); }
	static void addLine(glm::vec2 p1, glm::vec2 p2) { mapper->addLine(p1, p2); }
	static void addArrow(glm::vec2 p1, glm::vec2 p2, GLfloat headSize = 1.0f) { mapper->addArrow(p1, p2, headSize); }

	static void clear() { mapper->clear(); }

private:
	static std::shared_ptr<PrimitivesMapper> mapper;
};