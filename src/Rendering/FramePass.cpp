#include "FramePass.h"
#include "Framebuffer.h"

FramePass::FramePass(const std::string& passName) :
	RenderPass(passName)
{
	// A non-zero vao must be bound even if not using vertex attributes for a quad
	// vertices defined in shader
	glGenVertexArrays(1, &emptyVaoID);
}

FramePass::~FramePass()
{
	glDeleteVertexArrays(1, &emptyVaoID);
}

void FramePass::render(const std::string& shaderGroup)
{
	framebuffer->bind();

	// Clear the framebuffer, done differently for different passes
	clear();

	// Bind the inputs/fbos as textures
	bindInputs();

	// Render the quad
	glBindVertexArray(emptyVaoID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	framebuffer->unbind();
}