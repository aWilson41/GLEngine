//[VERTEX SHADER]
#version 330

in vec3 InVertex;
in vec3 InNormal;

smooth out vec4 Color;

// Model view projection
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat3 tInvModelViewMat; // Transpose(Inv(modelView))

// Material (containing diffuse, specular, and ambient light)
uniform mat4 mat;

// Directional Light
uniform vec3 dirLight;

void main()
{
    vec4 worldPos = modelMat * vec4(InVertex, 1.0);
    vec3 normal = normalize(tInvModelViewMat * InNormal);

    gl_Position = projMat * viewMat * worldPos;

    // Calculate diffuse light
    float kd = clamp(dot(normal, dirLight), 0.0, 1.0);
    vec4 diffuse = mat[0];

    /*vec4 specular = mat[1];
    vec4 ambient = mat[2];*/

    Color = kd * diffuse;
}