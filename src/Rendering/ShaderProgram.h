#pragma once
#include "PropertyMap.h"
#include <GL/glew.h>
#include <string>
#include <vector>

class ShaderFile
{
public:
	ShaderFile() { }
	ShaderFile(std::string fileName, GLenum shaderType) { load(fileName, shaderType); }

public:
	std::string getSrc() const { return srcStr; }
	std::string getFileName() const { return fileName; }
	GLuint getShaderID() const { return shaderID; }
	GLenum getShaderType() const { return shaderType; }

	void setID(GLuint id) { shaderID = id; }

	void load(std::string fileName, GLenum shaderType);
	void compile();

private:
	std::string read();

private:
	GLuint shaderID = -1;
	std::string srcStr = "";
	std::string fileName = "";
	GLenum shaderType = GL_FRAGMENT_SHADER;
};


class ShaderProgram
{
public:
	ShaderProgram(std::string name) { shaderName = name; }
	~ShaderProgram();

public:
	GLuint getProgramID() const { return programID; }
	std::string getName() const { return shaderName; }

	std::string readShaderFile(std::string filePath);
	void loadShader(std::string filePath, GLenum shaderType);
	void compileShader(const char* src, GLuint shaderID, std::string filePath);
	void compileProgram();
	void release();

protected:
	std::string shaderName = "unnamed";
	GLuint programID = -1;

	std::vector<ShaderFile*> files;
};