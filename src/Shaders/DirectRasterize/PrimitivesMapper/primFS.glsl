#version 460

struct PhongMaterial
{
	vec4 diffuseColor;
	vec4 specularColor;
	vec3 ambientColor;
};

uniform PhongMaterial mat;

out vec4 fColor;

void main()
{
	vec3 diffuseColor = mat.diffuseColor.rgb;
	vec3 ambientColor = mat.ambientColor.rgb;

	fColor = vec4(diffuseColor + ambientColor, mat.diffuseColor.a);
}