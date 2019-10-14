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
	fColor = vec4(mat.ambientColor, 1.0);
}