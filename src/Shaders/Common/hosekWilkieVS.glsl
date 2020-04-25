#version 460

uniform mat4 projMatrix;

layout (location = 0) in vec3 inPos;

out vec3 fragPos;

void main()
{
	fragPos = normalize(inPos);
	gl_Position = projMatrix * vec4(fragPos, 1.0f);
}