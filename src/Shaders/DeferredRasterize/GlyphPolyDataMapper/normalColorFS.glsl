#version 460
struct Material
{
	vec4 diffuseColor;
	vec3 ambientColor;
};

uniform Material mat;

smooth in vec3 pos;
smooth in vec3 normal;
smooth in vec3 color;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuseColor;
layout (location = 3) out vec4 gAmbientColor;

void main()
{
	gPosition = pos;
	gNormal = normal;
	gDiffuseColor = vec4(color, 1.0f);
	gAmbientColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}