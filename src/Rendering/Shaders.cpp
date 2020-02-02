#include "Shaders.h"
#include "AbstractMapper.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <string>

namespace Shaders
{
	static std::vector<std::string> splitStr(std::string str, std::string delimiter)
	{
		std::vector<std::string> results;
		size_t pos = 0;
		while ((pos = str.find(delimiter)) != std::string::npos)
		{
			results.push_back(str.substr(0, pos));
			//std::cout << token << std::endl;
			str.erase(0, pos + delimiter.length());
		}
		results.push_back(str);
		return results;
	}

	static std::string readFile(std::string fileName)
	{
		std::ifstream t(fileName);
		std::stringstream buffer;
		buffer << t.rdbuf();
		return buffer.str();
	}

	std::vector<ShaderProgram*> shaderCache;

	// Load shader program from source
	ShaderProgram* loadVSFSShader(std::string shaderName, std::string vsPath, std::string fsPath)
	{
		ShaderProgram* shader = new ShaderProgram(shaderName);
		shader->loadShader(vsPath, GL_VERTEX_SHADER);
		shader->loadShader(fsPath, GL_FRAGMENT_SHADER);
		shader->compileProgram();
		shaderCache.push_back(shader);
		return shader;
	}

	ShaderProgram* loadVSFSGSShader(std::string shaderName, std::string vsPath, std::string fsPath, std::string gsPath)
	{
		ShaderProgram* shader = new ShaderProgram(shaderName);
		shader->loadShader(vsPath, GL_VERTEX_SHADER);
		shader->loadShader(fsPath, GL_FRAGMENT_SHADER);
		shader->loadShader(gsPath, GL_GEOMETRY_SHADER);
		shader->compileProgram();
		shaderCache.push_back(shader);
		return shader;
	}

	ShaderProgram* loadComputeShader(std::string shaderName, std::string path)
	{
		ShaderProgram* shader = new ShaderProgram(shaderName);
		shader->loadShader(path, GL_COMPUTE_SHADER);
		shader->compileProgram();
		shaderCache.push_back(shader);
		return shader;
	}
	ShaderProgram* loadComputeShader(std::string shaderName, std::vector<std::string> paths)
	{
		ShaderProgram* shader = new ShaderProgram(shaderName);
		for (UINT i = 0; i < paths.size(); i++)
		{
			shader->loadShader(paths[i], GL_COMPUTE_SHADER);
		}
		shader->compileProgram();
		shaderCache.push_back(shader);
		return shader;
	}

	ShaderProgram* getShader(std::string shaderGroup, std::string mapperName, ShaderProperties* properties)
	{
		if (properties == nullptr)
			return nullptr;

		properties->update();

		const std::string shaderPath = "Shaders/" + shaderGroup + '/' + mapperName + '/';
		// Read the mapping file to find the correct shader
		const unsigned long long lineNum = properties->getKey();
		const std::string fileStr = readFile(shaderPath + "mappings.csv");
		const std::vector<std::string> lineStrs = splitStr(fileStr, "\n");
		const std::vector<std::string> shaderFileStrs = splitStr(lineStrs[lineNum], ",");
		if (shaderFileStrs.size() == 2)
			return loadVSFSShader("unnamed",
				shaderPath + shaderFileStrs[0],
				shaderPath + shaderFileStrs[1]);
		else if (shaderFileStrs.size() == 3)
			return loadVSFSGSShader("unnamed",
				shaderPath + shaderFileStrs[0],
				shaderPath + shaderFileStrs[1],
				shaderPath + shaderFileStrs[2]);
		else
			return nullptr;
	}

	void deleteShaders()
	{
		for (unsigned int i = 0; i < shaderCache.size(); i++)
		{
			shaderCache[i]->release();
			delete shaderCache[i];
		}
	}
};