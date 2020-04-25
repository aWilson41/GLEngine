#pragma once
#include "FramebufferAttachment.h"
#include <memory>

class Framebuffer
{
public:
	enum class AttachmentType
	{
		COLOR,
		DEPTH,
		STENCIL
	};
	using AttachmentConfig = std::tuple<AttachmentType, FramebufferAttachment::Format, void*>;

public:
	~Framebuffer()
	{
		if (fboID != -1)
			glDeleteFramebuffers(1, &fboID);
	}

public:
	std::shared_ptr<FramebufferAttachment> getAttachment(UINT port) const { return attachments[port]; }
	UINT getNumberOfAttachments() const { return static_cast<UINT>(attachments.size()); }
	glm::ivec2 getDim()
	{
		if (attachments.size() > 0)
			return attachments[0]->getDim();
		else
			return glm::ivec2(0);
	}

	bool resize(const UINT width, const UINT height) { return resize(glm::ivec2(width, height)); }
	bool resize(const glm::ivec2 dim);

	// Generate and add attachments (returns if succesful)
	bool generate(const UINT width, const UINT height, std::vector<AttachmentConfig> configs);
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

	void clearColor() { glClear(GL_COLOR_BUFFER_BIT); }
	void clearDepth() { glClear(GL_DEPTH_BUFFER_BIT); }
	void clearStencil() { glClear(GL_STENCIL_BUFFER_BIT); }
	void clear(GLbitfield mask) { glClear(mask); }
	void clear() { glClear(clearMask); }

private:
	GLuint fboID = -1;
	std::vector<std::shared_ptr<FramebufferAttachment>> attachments;
	std::vector<AttachmentConfig> attachmentConfigs;
	UINT numColorAttachments = 0;
	GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
};