#version 460

layout (location = 0) in vec3 inPos;

void main()
{
    // Just forward the position, projection will happen in the geometry shader
    gl_Position = vec4(inPos, 1.0);
}