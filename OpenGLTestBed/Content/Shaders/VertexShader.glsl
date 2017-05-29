//[VERTEX SHADER]
#version 330

in vec3 inVertex;
in vec3 inNormal;

out vec3 normal;

// Model view projection
uniform mat4 mvp; // proj * view * model
uniform mat3 tInvModel; // Transpose(Inv(model))

void main()
{
    normal = tInvModel * inNormal;

    gl_Position = mvp * vec4(inVertex, 1.0);
}