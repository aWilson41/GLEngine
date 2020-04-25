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
	~PrimitivesMapper() override;

public:
	std::shared_ptr<ShaderProgram> getShaderProgram() const { return shaderProgram; }
	std::shared_ptr<PhongMaterial> getMaterial() const { return material; }
	GLfloat getLineWidth() const { return lineWidth; }

	void setMaterial(std::shared_ptr<PhongMaterial> material) { this->material = material; }
	void setLineWidth(const GLfloat lineWidth) { this->lineWidth = lineWidth; }
	void setZ(const GLfloat z) { this->z = z; }

	void addCircle(const glm::vec2& pos, const GLfloat r, const UINT div);
	void addSphere(const glm::vec3& pos, const GLfloat r, const UINT divTheta, const UINT divPhi);
	void addLine(const glm::vec2& p1, const glm::vec2& p2) { addLine(glm::vec3(p1, z), glm::vec3(p2, z)); }
	void addLine(const glm::vec3& p1, const glm::vec3& p2);
	void addArrow(const glm::vec2& p1, const glm::vec2& p2, const GLfloat headSize = 1.0f, const GLfloat headRatio = 0.9f);

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

	GLfloat lineWidth = 0.1f;
	GLfloat z = -1.0f;

	std::shared_ptr<ShaderProgram> shaderProgram = nullptr;
};

// Class for debug annotation rendering, static mapper must be added to your renderer
class Primitives2d : public Singleton<Primitives2d>
{
public:
	static std::shared_ptr<PrimitivesMapper> getMapper() { return mapper; }

	static void setZ(const GLfloat z) { mapper->setZ(z); }
	static void setColor(const glm::vec3& color);
	static void setColor(const GLfloat r, const GLfloat g, const GLfloat b);

	static void addCircle(const glm::vec2& pos, const GLfloat r, const UINT div) { mapper->addCircle(pos, r, div); }
	static void addLine(const glm::vec2& p1, const glm::vec2& p2) { mapper->addLine(p1, p2); }
	static void addArrow(const glm::vec2& p1, const glm::vec2& p2, const GLfloat headSize = 1.0f, const GLfloat headRatio = 0.9f)
	{
		mapper->addArrow(p1, p2, headSize, headRatio);
	}

	static void clear() { mapper->clear(); }

private:
	static std::shared_ptr<PrimitivesMapper> mapper;
};
class Primitives3d : public Singleton<Primitives3d>
{
public:
	static std::shared_ptr<PrimitivesMapper> getMapper() { return mapper; }

	static void setColor(const glm::vec3& color);
	static void setColor(const GLfloat r, const GLfloat g, const GLfloat b);

	static void addSphere(const glm::vec3& pos, const GLfloat r, const UINT divTheta, const UINT divPhi) { mapper->addSphere(pos, r, divTheta, divPhi); }

	static void clear() { mapper->clear(); }

private:
	static std::shared_ptr<PrimitivesMapper> mapper;
};