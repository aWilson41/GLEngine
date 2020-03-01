#pragma once
#include "MathHelper.h"

class FramebufferAttachment
{
public:
	enum class Format
	{
		RGB16F,
		RGB32F,
		R32F,
		RGBA,
		RGBA32F,
		DEPTH32F
	};

public:
	FramebufferAttachment() { }
	FramebufferAttachment(Format format, UINT width, UINT height, void* data = NULL)
	{
		generate(format, width, height, data);
	}
	~FramebufferAttachment()
	{
		if (isGenerated())
			glDeleteTextures(1, &texID);
	}

public:
	glm::ivec2 getDim() const { return dim; }

	void resize(const UINT width, const UINT height) { generate(format, width, height, data); }

	void generate(const Format format, const UINT width, const UINT height, void* data = NULL);

	void bind(UINT location)
	{
		glActiveTexture(GL_TEXTURE0 + location);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	void clearTex();
	void clearTex(const GLfloat val);
	void clearTex(const glm::vec3& val);
	void clearTex(const glm::vec4& val);
	void clearTex(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
	void clearDepth(const GLfloat val);

	bool isGenerated() { return texID != -1; }

private:
	friend class Framebuffer;

	GLuint texID = -1;

	glm::ivec2 dim = glm::ivec2(100, 100);
	Format format = Format::RGB32F;
	void* data = nullptr;
};