#pragma once
#include "ShaderProgram.h"
#include <memory>
#include <vector>

class Renderer;
class ShaderProperties;

namespace Shaders
{
	extern std::shared_ptr<ShaderProgram> loadVSFSShader(std::string shaderName, std::string vsPath, std::string fsPath);
	extern std::shared_ptr<ShaderProgram> loadVSFSGSShader(std::string shaderName, std::string vsPath, std::string fsPath, std::string gsPath);
	extern std::shared_ptr<ShaderProgram> loadComputeShader(std::string shaderName, std::string path);
	extern std::shared_ptr<ShaderProgram> loadComputeShader(std::string shaderName, std::vector<std::string> paths);

	extern std::shared_ptr<ShaderProgram> getShader(std::string shaderGroup, std::string mapperName, ShaderProperties* properties);
};