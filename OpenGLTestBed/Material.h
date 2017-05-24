#pragma once
#include "Texture.h"
#include "MathHelper.h"
#include <GL\glew.h>

class Material
{
public:
    // Colum order diffuse, specular, ambient
    glm::mat4 mat;
    GLuint matID;

	Texture* diffuseMap;
	bool hasDiffuseMap;

public:
	Material();
    glm::vec4 GetDiffuseColor();
    glm::vec4 GetSpecularColor();
    glm::vec4 GetAmbientColor();
    void SetDiffuseColor(glm::vec4 diffuse);
    void SetSpecularColor(glm::vec4 specular);
    void SetSpecularShine(float specularShine) { mat[2][3] = specularShine; }
    void SetAmbientColor(glm::vec4 ambient);
};