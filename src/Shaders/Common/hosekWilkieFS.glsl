#version 460

uniform vec3 params[10];
uniform vec3 lightDir;

out vec4 fColor;

in vec3 fragPos;

vec3 HosekWilkie(float cosTheta, float gamma, float cosGamma)
{
	vec3 A = params[0];
	vec3 B = params[1];
	vec3 C = params[2];
	vec3 D = params[3];
	vec3 E = params[4];
	vec3 F = params[5];
	vec3 G = params[6];
	vec3 H = params[7];
	vec3 I = params[8];
	vec3 Z = params[9];
	vec3 chi = (1.0f + cosGamma * cosGamma) / pow(1.0f + H * H - 2.0f * cosGamma * H, vec3(1.5f));
    return (1.0f + A * exp(B / (cosTheta + 0.01f))) * (C + D * exp(E * gamma) + F * (cosGamma * cosGamma) + G * chi + I * sqrt(cosTheta));
}

void main()
{
	vec3 V = normalize(fragPos);
	float cosTheta = clamp(V.y, 0.0f, 1.0f);
	float cosGamma = dot(V, lightDir);
	float gamma = acos(cosGamma);

	vec3 Z = params[9];
	vec3 R = Z * HosekWilkie(cosTheta, gamma, cosGamma);
	if (cosGamma > 0)
		R = R + pow(vec3(cosGamma), vec3(256)) * 0.5f;

	fColor = vec4(R, 1.0f);
}