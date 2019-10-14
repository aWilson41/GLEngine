#version 460
#define M_PI 3.1415926535897932384626433832795

layout(binding = 0) uniform sampler2D inputTex;

uniform int blurRadius;
uniform float sigmaI;
uniform float sigmaS;

smooth in vec2 texCoord;

out vec3 fragColor;

float gaussian(float x, float sigma)
{
	float a = 2.0f * sigma * sigma;
    return exp(-(x * x) / a) / (M_PI * a);
}

void main()
{
    vec3 rgb = texture(inputTex, texCoord).rgb;
    vec2 dim = textureSize(inputTex, 0);

	vec3 sum = vec3(0.0f);
	vec3 weightSum = vec3(0.0f);
	for (int j = -blurRadius; j < blurRadius; j++)
	{
		for (int i = -blurRadius; i < blurRadius; i++)
		{
			vec2 dx = vec2(float(i), float(j)) / dim;
			vec2 pos = texCoord + dx;

			// Gaussian weight given the difference of intensity
			vec3 currRgb = texture(inputTex, pos).rgb;
			vec3 diffRgb = currRgb - rgb;
			vec3 gi = vec3(gaussian(diffRgb.x, sigmaI), gaussian(diffRgb.y, sigmaI), gaussian(diffRgb.z, sigmaI));

			// Gaussian weight given the difference in position
			float dist = sqrt(dx.x * dx.x + dx.y * dx.y);
			float gs = gaussian(dist, sigmaS);

			// Then convolve
			vec3 w = gi * gs;
			sum += currRgb * w;
			weightSum += w;
		}
	}
    fragColor = sum / weightSum;
}