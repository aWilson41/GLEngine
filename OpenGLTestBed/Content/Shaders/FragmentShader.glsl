//[FRAGMENT SHADER]
#version 330

in vec3 outNormal;
in vec3 outCam;

out vec4 fragColor;

// Material (containing diffuse, specular, and ambient light)
uniform mat4 mat;

// Directional Light
uniform vec3 dirLight;

void main()
{
    // Calculate diffuse light
    vec4 diffuse = mat[0];
    float kd = clamp(dot(outNormal, dirLight), 0.0, 1.0);

    // Calculate specular light
    vec4 specular = mat[1];
    float shine = specular[3];
    specular[3] = 1.0;
    vec3 r = reflect(dirLight, outNormal);
    float ks = pow(clamp(dot(outCam, r), 0.0, 1.0), shine);

    // Ambient light
    vec4 ambient = mat[2];

    fragColor = kd * diffuse + ks * specular + ambient;
}