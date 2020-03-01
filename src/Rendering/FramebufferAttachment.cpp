#include "FramebufferAttachment.h"

void FramebufferAttachment::generate(const Format format, const UINT width, const UINT height, void* data)
{
	this->format = format;
	this->data = data;
	dim = glm::ivec2(width, height);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	switch (format)
	{
	case Format::RGB16F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
		break;
	case Format::RGB32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);
		break;
	case Format::R32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
		break;
	case Format::RGBA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	case Format::RGBA32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);
		break;
	case Format::DEPTH32F:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
		break;
	default:
		break;
	};
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void FramebufferAttachment::clearTex()
{
	switch (format)
	{
	case Format::RGB16F:
		clearTex(glm::vec3(0.0f));
		break;
	case Format::RGB32F:
		clearTex(glm::vec3(0.0f));
		break;
	case Format::R32F:
		clearTex(0.0f);
		break;
	case Format::RGBA:
		clearTex(0, 0, 0, 255);
		break;
	case Format::RGBA32F:

		break;
	case Format::DEPTH32F:
		clearDepth(1.0f);
		break;
	default:
		break;
	};
}
void FramebufferAttachment::clearTex(const GLfloat val)
{
	if (format == Format::RGB32F)
		glClearTexImage(texID, 0, GL_RED, GL_FLOAT, &val);
}
void FramebufferAttachment::clearTex(const glm::vec3& val)
{
	if (format == Format::RGB16F || format == Format::RGB32F)
		glClearTexImage(texID, 0, GL_RGB, GL_FLOAT, &val);
}
void FramebufferAttachment::clearTex(const glm::vec4& val)
{
	if (format == Format::RGBA32F)
		glClearTexImage(texID, 0, GL_RGBA, GL_FLOAT, &val);
}
void FramebufferAttachment::clearTex(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	if (format == Format::RGBA)
	{
		GLubyte clearColor[4] = { r, g, b, a };
		glClearTexImage(texID, 0, GL_RGB, GL_UNSIGNED_BYTE, clearColor);
	}
}
void FramebufferAttachment::clearDepth(const GLfloat val)
{
	if (format == Format::DEPTH32F)
		glClearTexImage(texID, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &val);
}