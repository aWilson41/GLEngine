#version 460
uniform mat4 mvp_matrix;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 3) in vec3 inColor;

smooth out vec3 normal;
smooth out vec3 color;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(inPos, 1.0);
	normal = inNormal;
	color = inColor;
}