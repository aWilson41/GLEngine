#version 460

struct PhongMaterial
{
	vec3 diffuseColor;
	vec4 specularColor;
	vec3 ambientColor;
};

uniform PhongMaterial mat;

out vec4 fColor;

void main()
{
	vec3 diffuseColor = mat.diffuseColor;
	vec3 ambientColor = mat.ambientColor;

	fColor = vec4(diffuseColor + ambientColor, 1.0);
}