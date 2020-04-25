#include "Framebuffer.h"
#include <tuple>

bool Framebuffer::resize(const glm::ivec2 dim)
{
	//generate(dim.x, dim.y, this->attachmentConfigs);
	/*if (isGenerated())
	{
		glDeleteFramebuffers(1, &fboID);

	}
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	for (UINT i = 0; i < attachments.size(); i++)
	{
		attachments[i]->resize(dim.x, dim.y);
	}*/

	//const bool complete = isComplete();

	// Back to the default fbo
	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	return generate(dim.x, dim.y, this->attachmentConfigs);
}

// Generate and add attachments (returns if succesful)
bool Framebuffer::generate(const UINT width, const UINT height, std::vector<AttachmentConfig> configs)
{
	if (isGenerated())
	{
		glDeleteFramebuffers(1, &fboID);
		attachments.clear();
	}
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	std::vector<UINT> attachmentTypes(configs.size());
	clearMask = 0;
	this->attachmentConfigs = configs;
	for (UINT i = 0; i < configs.size(); i++)
	{
		const AttachmentType attachmentType = std::get<0>(configs[i]);
		const FramebufferAttachment::Format format = std::get<1>(configs[i]);
		void* dataPtr = std::get<2>(configs[i]);
		std::shared_ptr<FramebufferAttachment> attachment =
			std::make_shared<FramebufferAttachment>(format, width, height, dataPtr);
		attachments.push_back(attachment);
		if (attachmentType == AttachmentType::COLOR)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_TEXTURE_2D, attachment->texID, 0);
			attachmentTypes[i] = GL_COLOR_ATTACHMENT0 + numColorAttachments;
			numColorAttachments++;
			clearMask |= GL_COLOR_BUFFER_BIT;
		}
		else if (attachmentType == AttachmentType::DEPTH)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment->texID, 0);
			clearMask |= GL_DEPTH_BUFFER_BIT;
		}
		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachment->texID, 0);
			clearMask |= GL_STENCIL_BUFFER_BIT;
		}
	}

	glDrawBuffers(numColorAttachments, attachmentTypes.data());

	const bool complete = isComplete();

	// Back to the default fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return complete;
}