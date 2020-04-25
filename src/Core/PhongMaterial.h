#pragma once
#include "MathHelper.h"

class PhongMaterial
{
public:
	PhongMaterial()
	{
		diffuseColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		specularColor = glm::vec4(0.0f);
		ambientColor = glm::vec3(0.0f);
	}
	PhongMaterial(const glm::vec4& diffuseColor, const glm::vec4& specularColor, const glm::vec3& ambientColor)
	{
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;
		this->ambientColor = ambientColor;
	}
	PhongMaterial(const glm::vec4& diffuseColor, GLfloat fraction)
	{
		this->diffuseColor = diffuseColor;
		specularColor = glm::vec4(0.0f);
		ambientColor = glm::vec3(diffuseColor) * fraction;
	}

public:
	// Sets the ambient color to a fraction of the diffuse
	void setAmbientToDiffuse(GLfloat fraction = 1.0f) { ambientColor = diffuseColor * fraction; }
	void setDiffuse(const glm::vec3& diffuseColor) { this->diffuseColor = glm::vec4(diffuseColor, 1.0f); }
	void setAlpha(GLfloat alpha) { diffuseColor.z = alpha; }
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b) { setDiffuse(glm::vec3(r, g, b)); }
	void setSpecular(const glm::vec4& specularColor) { PhongMaterial::specularColor = specularColor; }
	void setAmbient(const glm::vec3& ambientColor) { PhongMaterial::ambientColor = ambientColor; }
	void setAmbient(GLfloat r, GLfloat g, GLfloat b) { setAmbient(glm::vec3(r, g, b)); }

	glm::vec4 getDiffuse() const { return diffuseColor; }
	glm::vec4 getSpecular() const { return specularColor; }
	glm::vec3 getAmbient() const { return ambientColor; }

protected:
	glm::vec4 diffuseColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 specularColor = glm::vec4(0.0f);
	glm::vec3 ambientColor = glm::vec3(0.0f);
};