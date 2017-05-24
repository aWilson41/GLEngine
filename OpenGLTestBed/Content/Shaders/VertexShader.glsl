//[VERTEX SHADER]
#version 330

in vec3 inVertex;
in vec3 inNormal;

out vec3 outNormal;
out vec3 outCam;

// Model view projection
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat3 tInvModelViewMat; // Transpose(Inv(modelView))

//// Material (containing diffuse, specular, and ambient light)
//uniform mat4 mat;
//
//// Directional Light
//uniform vec3 dirLight;

void main()
{
    vec4 worldPos = modelMat * vec4(inVertex, 1.0);
    outNormal = normalize(tInvModelViewMat * inNormal);

    gl_Position = projMat * viewMat * worldPos;

    // Calculate diffuse light
    //vec4 diffuse = mat[0];
    //float kd = clamp(dot(normal, dirLight), 0.0, 1.0);

    //// Calculate specular light
    //vec4 specular = mat[1];
    //float shine = specular[3];
    //specular[3] = 1.0;
    //vec3 r = reflect(normal, dirLight);
    //vec3 v = vec3(viewMat[0][2], viewMat[1][2], viewMat[2][2]);
    //float ks = pow(clamp(dot(v, r), 0.0, 1.0), shine);

    outCam = vec3(viewMat[0][2], viewMat[1][2], viewMat[2][2]);
}