#pragma once
#include "MathHelper.h"
#include <memory>
#include <unordered_map>

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

	void generate(const Format format, const UINT width, const UINT height, void* data = NULL)
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

	void bind(UINT location)
	{
		glActiveTexture(GL_TEXTURE0 + location);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	void clearTex()
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
	void clearTex(const GLfloat val)
	{
		if (format == Format::RGB32F)
			glClearTexImage(texID, 0, GL_RED, GL_FLOAT, &val);
	}
	void clearTex(const glm::vec3& val)
	{
		if (format == Format::RGB16F || format == Format::RGB32F)
			glClearTexImage(texID, 0, GL_RGB, GL_FLOAT, &val);
	}
	void clearTex(const glm::vec4& val)
	{
		if (format == Format::RGBA32F)
			glClearTexImage(texID, 0, GL_RGBA, GL_FLOAT, &val);
	}
	void clearTex(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	{
		if (format == Format::RGBA)
		{
			GLubyte clearColor[4] = { r, g, b, a };
			glClearTexImage(texID, 0, GL_RGB, GL_UNSIGNED_BYTE, clearColor);
		}
	}
	void clearDepth(const GLfloat val)
	{
		if (format == Format::DEPTH32F)
			glClearTexImage(texID, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &val);
	}

	bool isGenerated() { return texID != -1; }

private:
	friend class Framebuffer;

	GLuint texID = -1;

	glm::ivec2 dim = glm::ivec2(-1, -1);
	Format format = Format::RGB32F;
	void* data = nullptr;
};

class Framebuffer
{
public:
	enum class AttachmentType
	{
		COLOR,
		DEPTH,
		STENCIL
	};
	using Config = std::tuple<AttachmentType, FramebufferAttachment::Format, void*>;

public:
	~Framebuffer()
	{
		if (fboID != -1)
			glDeleteFramebuffers(1, &fboID);
	}

public:
	std::shared_ptr<FramebufferAttachment> getAttachment(UINT port) const { return attachments[port]; }

	bool resize(const UINT width, const UINT height) { return resize(glm::ivec2(width, height)); }
	bool resize(const glm::ivec2 dim)
	{
		if (isGenerated())
			glDeleteFramebuffers(1, &fboID);
		glGenFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		for (UINT i = 0; i < attachments.size(); i++)
		{
			attachments[i]->resize(dim.x, dim.y);
		}

		const bool complete = isComplete();

		// Back to the default fbo
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return complete;
	}

	// Generate and add attachments (returns if succesful)
	bool generate(const UINT width, const UINT height, std::vector<Config> config)
	{
		if (isGenerated())
		{
			glDeleteFramebuffers(1, &fboID);
			attachments.clear();
		}
		glGenFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);

		std::vector<UINT> attachmentTypes(config.size());
		for (UINT i = 0; i < config.size(); i++)
		{
			const AttachmentType attachmentType = std::get<0>(config[i]);
			const FramebufferAttachment::Format format = std::get<1>(config[i]);
			void* dataPtr = std::get<2>(config[i]);
			std::shared_ptr<FramebufferAttachment> attachment =
				std::make_shared<FramebufferAttachment>(format, width, height, dataPtr);
			attachments.push_back(attachment);
			if (attachmentType == AttachmentType::COLOR)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_TEXTURE_2D, attachment->texID, 0);
				attachmentTypes[i] = GL_COLOR_ATTACHMENT0 + numColorAttachments;
				numColorAttachments++;
			}
			else if (attachmentType == AttachmentType::DEPTH)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment->texID, 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachment->texID, 0);
		}

		glDrawBuffers(numColorAttachments, attachmentTypes.data());

		const bool complete = isComplete();

		// Back to the default fbo
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return complete;
	}
	// Returns true if complete
	bool isComplete() { return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE); }
	bool isGenerated() { return fboID != -1; }

	void bind() { glBindFramebuffer(GL_FRAMEBUFFER, fboID); }
	void bindRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID); }
	void bindAttachments()
	{
		for (UINT i = 0; i < attachments.size(); i++)
		{
			attachments[i]->bind(i);
		}
	}
	void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void clearColor()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void clearDepth()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		glClear(GL_COLOR_BUFFER_BIT);
	}

private:
	GLuint fboID = -1;
	std::vector<std::shared_ptr<FramebufferAttachment>> attachments;
	UINT numColorAttachments = 0;
};