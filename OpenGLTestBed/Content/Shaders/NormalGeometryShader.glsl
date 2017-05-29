#version 330
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;


in vec4 position;
in vec4 normal;

out vec4 color;

uniform mat4 mvp; // proj * view * model

void main()
{
    const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    const vec4 blue = vec4(0.0f, 0.0f, 1.0f, 1.0f);

    for (int i = 0; i < 3; i++)
    {
        gl_Position = vdata[i].mvp * vdata[i].position;
        color = green;
        EmitVertex();

        gl_Position = vdata[i].mvp * (vdata[i].position + vdata[i].normal);
        color = blue;
        EmitVertex();

        EndPrimitive();
    }
}