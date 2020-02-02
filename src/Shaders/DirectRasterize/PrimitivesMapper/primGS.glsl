#version 460

uniform mat4 mvp_matrix;
uniform vec3 viewDir;
uniform float lineThickness = 1.0f;

// Takes a line and produces 4 vertices (2 triangles/quad)
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

void main()
{
    float w = gl_in[0].gl_Position.w;

    vec3 p1 = vec3(gl_in[0].gl_Position);
    vec3 p2 = vec3(gl_in[1].gl_Position);

    // Get normal between two vertices
    vec3 tan = normalize(cross(viewDir, p2 - p1)) * lineThickness;

    gl_Position = mvp_matrix * vec4(p1 + tan, w);
    EmitVertex();

    gl_Position = mvp_matrix * vec4(p2 + tan, w);
    EmitVertex();

    gl_Position = mvp_matrix * vec4(p1 - tan, w);
    EmitVertex();

    gl_Position = mvp_matrix * vec4(p2 - tan, w);
    EmitVertex();
    
    EndPrimitive();
}