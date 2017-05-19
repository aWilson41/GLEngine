#pragma once
#include "Scene.h"
#define NOMINMAX
#include <Windows.h>
#include "LodePNG\lodepng.h"
#include <fstream>
#include <iostream>

// Assimp
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

class ResourceLoader
{
public:
    static void LoadAllMeshesFromFolder(const std::string path, Scene* scene)
    {
        WIN32_FIND_DATA fd;
        HANDLE hFind = FindFirstFile(path.c_str(), &fd);
        Assimp::Importer importer;
        if (hFind != INVALID_HANDLE_VALUE) 
        {
            do {
                // read all (real) files in current folder
                // , delete '!' read other 2 default folder . and ..
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {					
                    std::string filePath = path.substr(0, path.size() - 1) + fd.cFileName;
                    const aiScene* aiscene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
                    LoadSceneFromAIScene(aiscene, scene);
                }
            } while (FindNextFile(hFind, &fd));

            FindClose(hFind);
        }
    }

    static void LoadSceneFromAIScene(const aiScene* aiscene, Scene* scene)
    {
        if (aiscene != nullptr)
        {
            // Load lightsw
            if (aiscene->HasLights())
            {
                for (UINT i = 0; i < aiscene->mNumLights; i++)
                {
                    aiLight light = aiscene->mLights[0][i];
                    if (light.mType == aiLightSource_DIRECTIONAL)
                        scene->dirLight.push_back(float4(light.mDirection.x, light.mDirection.y, light.mDirection.z, 0.0f));
                }
            }

            if (aiscene->HasMaterials())
            {
                // Load materials
                scene->material.resize(aiscene->mNumMaterials);
                std::vector<std::string> textureFilePaths;
                for (UINT i = 0; i < aiscene->mNumMaterials; i++)
                {
                    // Get the diffuse color
                    aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
                    aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
                    scene->material[i].diffuse = Color((char)(diffuseColor.r * 255.0f), (char)(diffuseColor.g * 255.0f), (char)(diffuseColor.b * 255.0f));

                    // Get the specular color
                    aiColor3D specularColor(0.0f, 0.0f, 0.0f);
                    aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
                    scene->material[i].specular = Color((char)(specularColor.r * 255.0f), (char)(specularColor.g * 255.0f), (char)(specularColor.b * 255.0f));

                    // Get the ambient color and shine
                    aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
                    aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
                    scene->material[i].ambient = Color((char)(ambientColor.r * 255.0f), (char)(ambientColor.g * 255.0f), (char)(ambientColor.b * 255.0f));
                    aiscene->mMaterials[i]->Get(AI_MATKEY_SHININESS, scene->material[i].specularShine);

                    // Get the diffuse texture map
                    aiString filePath;
                    UINT texCount = aiscene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
                    // Load texture into the scene (also creates a reference in material i)
                    if (texCount > 0 && aiscene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &filePath) == AI_SUCCESS)
                        textureFilePaths.push_back(filePath.C_Str());
                }

                // Load textures
                scene->texture.resize(textureFilePaths.size());
                for (UINT i = 0; i < scene->texture.size(); i++)
                {
                    LoadTexture(scene, textureFilePaths[i], DIFFUSEMAP, i);
                    scene->material[i].diffuseMap = &scene->texture[i];
                    scene->material[i].hasDiffuseMap = true;
                }
            }

            // Load Meshes
            if (aiscene->HasMeshes())
            {
                for (UINT i = 0; i < aiscene->mNumMeshes; i++)
                {
                    Mesh mesh;
                    mesh.name = aiscene->mMeshes[i]->mName.C_Str();

                    // Material information is already loaded in
                    // The mesh just needs some direction to it
                    UINT matIndex = aiscene->mMeshes[i]->mMaterialIndex;
                    mesh.mat = &scene->material[matIndex];

                    // Size the arrays
                    std::vector<VertexColor> vertices = std::vector<VertexColor>(aiscene->mMeshes[i]->mNumVertices);

                    // Get the vertices, vertex normals 
                    for (UINT j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
                    {
                        // Vertices
                        vertices[j].x = aiscene->mMeshes[i]->mVertices[j].x / 10.0f;
                        vertices[j].y = aiscene->mMeshes[i]->mVertices[j].y / 10.0f;
                        vertices[j].z = aiscene->mMeshes[i]->mVertices[j].z / 10.0f;
                        vertices[j].r = 0.5f;
                        vertices[j].g = 0.5f;
                        vertices[j].b = 0.5f;
                        vertices[j].a = 1.0f;
                    }

                    // Indices
                    std::vector<GLuint> indices = std::vector<GLuint>(aiscene->mMeshes[i]->mNumVertices);
                    for (UINT j = 0; j < aiscene->mMeshes[i]->mNumFaces; j++)
                    {
                        for (UINT k = 0; k < aiscene->mMeshes[i]->mFaces[j].mNumIndices; k++)
                        {
                            indices[j * 3 + k] = aiscene->mMeshes[i]->mFaces[j].mIndices[k];
                        }
                    }

                    mesh.SetVertexColorBuffer(vertices);
                    mesh.SetIndexBuffer(indices);
                    scene->mesh.push_back(mesh);
                }
            }
        }
        else
            std::cout << "Unable to load scene" << std::endl;
    }


    // Loads the entire directory as one shader (not finished)
    static GLuint LoadShaderFromFolder(const std::string path, GLenum shaderType)
    {
        std::string sources;
        std::vector<GLint> sourceLengths;
      
        WIN32_FIND_DATA fd;
        HANDLE hFind = FindFirstFile(path.c_str(), &fd);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do {
                // read all (real) files in current folder
                // , delete '!' read other 2 default folder . and ..
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    std::string filePath = path.substr(0, path.size() - 1) + fd.cFileName;
                    std::string source = ReadFileStr(filePath);
                    sources += source;
                    sourceLengths.push_back((GLint)source.length());
                }
            } while (FindNextFile(hFind, &fd));

            FindClose(hFind);
        }
      
        GLuint shaderID = glCreateShader(shaderType);
        GLchar* glSource = &sources[0];
        glShaderSource(shaderID, sourceLengths.size(), &glSource, &sourceLengths[0]);
        glCompileShader(shaderID);

        return shaderID;
    }

    // Loads a single file as a shader
    static GLuint LoadShaderFromPath(std::string path, GLenum shaderType)
    {
        std::string source = ReadFileStr(path);
        GLint length = source.size();

        GLuint shaderID = glCreateShader(shaderType);
        GLchar* glSource = &source[0];    
        glShaderSource(shaderID, 1, &glSource, &length);
        glCompileShader(shaderID);

        int wasCompiled = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &wasCompiled);
        if (wasCompiled == 0)
            PrintShaderCompilationErrorInfo(shaderID);

        return shaderID;
    }

    static void PrintShaderCompilationErrorInfo(int32_t shaderId)
    {
        std::cout << "=======================================\n";
        std::cout << "Shader compilation failed : " << std::endl;

        // Find length of shader info log
        int maxLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        // Get shader info log
        char* shaderInfoLog = new char[maxLength];
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog);

        // Print shader info log
        std::cout << "\tError info : " << shaderInfoLog << std::endl;

        std::cout << "=======================================\n\n";
        delete shaderInfoLog;
    }


    // Reads a text file into a string from path
    static std::string ReadFileStr(std::string path)
    {
        std::string source;

        // Read the text
        std::ifstream file;
        file.open(path);
        if (file.is_open())
        {
            while (!file.eof())
            {
                source += file.get();
            }
            file.close();
        }
        source.erase(source.end() - 1, source.end());
        return source;
    }


	// Adds texture to scene
	static bool LoadTexture(Scene*& scene, std::string filePath, TextureType texType, int textureIndex)
	{
        bool results = false;
        // Remove all the preceding text in the texture filePath
        //int pos = filePath.find_last_of('\\') + 1;
        int pos = filePath.find_last_of('//') + 1;
        filePath = filePath.substr(pos, filePath.size() - pos);
        std::string name = filePath.substr(0, filePath.length() - 4);

        // If the texture doesn't already exist
        int texIndex = scene->FindTexture(name);
        if (texIndex == -1)
        {
	        filePath = "Content/Textures/" + filePath;

	        // Load the png using LodePng
	        std::vector<unsigned char> image;
	        UINT width, height;
	        UINT error = lodepng::decode(image, width, height, filePath);
	        //std::string errorStr = lodepng_error_text(error);

	        if (error == 0)
	        {
		        // Parse the image so I can use it as a pointer
		        unsigned char* buffer = new unsigned char[width * height * 3];
		        for (UINT y = 0; y < height; y++)
		        {
			        for (UINT x = 0; x < width; x++)
			        {
				        UINT i = y * width + x;
				        UINT j = i * 3;
				        UINT k = i * 4;
				        buffer[j] = image[k];
				        buffer[j + 1] = image[k + 1];
				        buffer[j + 2] = image[k + 2];
			        }
		        }

		        // Add the texture to the scene
		        scene->texture[textureIndex] = Texture(name, width, height, buffer, texType);
		        results = true;
	        }
        }

        return results;
	}
};