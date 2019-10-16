#version 460
uniform sampler2D tex;

smooth in vec2 texCoord;

out vec4 fColor;

void main()
{
	float val = texture2D(tex, texCoord).r;
	fColor = vec4(val, val, val, 1.0f);
}