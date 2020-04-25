#version 460
struct Material
{
	vec4 diffuseColor;
	vec3 ambientColor;
};

uniform Material mat;
uniform vec3 lightDir;

smooth in vec3 normal;

out vec4 fColor;

void main()
{
	float diffuse = dot(lightDir, normal);

	vec3 color = clamp(diffuse * mat.diffuseColor.rgb + mat.ambientColor, 0.0f, 1.0f);
	fColor = vec4(color, mat.diffuseColor.a);
}