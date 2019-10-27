#pragma once
#include "MathHelper.h"
#include <string>

class DeferredRenderer;

class RenderPass
{
public:
	RenderPass(std::string name) { passName = name; }

public:
	GLuint getNumberOfInputPorts() const { return static_cast<GLuint>(inputs.size()); }
	GLuint getNumberOfOutputPorts() const { return static_cast<GLuint>(outputs.size()); }
	GLuint* getInput(size_t portNum) const { return inputs[portNum]; };
	GLuint getOutput(size_t portNum) const { return outputs[portNum]; }
	std::vector<GLuint*> getInputs() const { return inputs; }
	std::vector<GLuint> getOutputs() const { return outputs; }
	std::string getPassName() const { return passName; }
	GLuint getFramebuffer() const { return fboID; }

	void setPassDim(int width, int height)
	{
		fboWidth = width;
		fboHeight = height;
	}
	void setInput(size_t portNum, GLuint* glId) { inputs[portNum] = glId; }
	void setNumberOfInputPorts(GLuint numberOfPorts)
	{
		inputs.resize(numberOfPorts);
		std::fill_n(inputs.data(), inputs.size(), nullptr);
	}
	void setNumberOfOutputPorts(GLuint numberOfPorts) { outputs.resize(numberOfPorts); }

	virtual void render(DeferredRenderer* ren) = 0;
	virtual void resizeFramebuffer(int width, int height) = 0;

protected:
	GLuint fboID = -1;
	std::string passName = "Unnamed";
	std::vector<GLuint*> inputs;
	std::vector<GLuint> outputs;
	int fboWidth = 100;
	int fboHeight = 100;
};