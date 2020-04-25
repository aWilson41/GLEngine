#pragma once
#include "AbstractMapper.h"
#include <memory>

class ShaderProgram;

// This class maps the sky! (Hosek Wilki model)
class SkyMapper : public AbstractMapper
{
public:
	~SkyMapper() override;

public:
	glm::vec3 getLightDir() const { return lightDir; }

	void setLightDir(const glm::vec3& lightDir) { this->lightDir = lightDir; }

	// Updates the buffer based on the set poly data. If it's the first time it initializes the buffer
	void update() override;

	bool useShader(std::string shaderGroup) override;
	void draw(Renderer* ren) const override;

protected:
	std::shared_ptr<ShaderProgram> shaderProgram = nullptr;

	GLuint vboID = -1;
	GLuint iboID = -1;
	GLuint vaoID = -1;

	glm::vec3 params[10];
	GLfloat turbidity = 4.0f;
	glm::vec3 albedo = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 lightDir = glm::vec3(0.0f);
};