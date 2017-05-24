#include "Material.h"

Material::Material() : hasDiffuseMap(false)
{
    mat = glm::mat4();

    // Diffuse
    mat[0] = glm::vec4(0.5, 0.5, 0.5, 1.0);
    // Specular
    mat[1] = glm::vec4(0.0, 0.0, 0.0, 1.0);
    // Ambient
    mat[2] = glm::vec4(0.0, 0.0, 0.0, 1.0);
}

glm::vec4 Material::GetDiffuseColor()
{
    return mat[0];
}

glm::vec4 Material::GetSpecularColor()
{
    return mat[1];
}

glm::vec4 Material::GetAmbientColor()
{
    return mat[2];
}

void Material::SetDiffuseColor(glm::vec4 diffuse)
{
    mat[0] = diffuse;
}

void Material::SetSpecularColor(glm::vec4 specular)
{
    mat[1] = specular;
}

void Material::SetAmbientColor(glm::vec4 ambient)
{
    mat[2] = ambient;
}