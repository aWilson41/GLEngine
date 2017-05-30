#pragma once
#include "Scene.h"
#include "LodePNG\lodepng.h"
#define NOMINMAX
#include <Windows.h>
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
				// read all (real) files in current folder, delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{					
					std::string filePath = path.substr(0, path.size() - 1) + fd.cFileName;
					const aiScene* aiscene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
					Scene fileScene;
					LoadSceneFromAIScene(aiscene, &fileScene);
					scene->merge(&fileScene);
				}
			} while (FindNextFile(hFind, &fd));

			FindClose(hFind);
		}
	}

    static void LoadSceneFromAIScene(const aiScene* aiscene, Scene* scene)
    {
        if (aiscene != nullptr)
        {
            // Load lights
            /*if (aiscene->HasLights())
            {
                for (UINT i = 0; i < aiscene->mNumLights; i++)
                {
                    aiLight light = aiscene->mLights[0][i];
                    if (light.mType == aiLightSource_DIRECTIONAL)
                        scene->dirLight.push_back(float4(light.mDirection.x, light.mDirection.y, light.mDirection.z, 0.0f));
                }
            }*/

            if (aiscene->HasMaterials())
            {
                // Load materials. (texture path, index of all the objects using it)
				std::map<std::string, std::vector<UINT>> texPath;
                for (UINT i = 0; i < aiscene->mNumMaterials; i++)
                {
					Material* mat = new Material();
                    // Get the diffuse color
                    aiColor4D diffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
                    aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
                    mat->SetDiffuseColor(glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a));

                    // Get the specular color
                    aiColor4D specularColor(0.0f, 0.0f, 0.0f, 1.0f);
                    aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
                    float shine = 1.0f;
                    aiscene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shine);
                    mat->SetSpecularColor(glm::vec4(specularColor.r, specularColor.g, specularColor.b, shine));

                    // Get the ambient color and shine
                    aiColor4D ambientColor(0.0f, 0.0f, 0.0f, 1.0f);
                    aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
                    mat->SetAmbientColor(glm::vec4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a));

                    // Save the diffuse texture path
                    UINT texCount = aiscene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
					if (texCount > 0)
					{
						aiString filePath;
						mat->hasDiffuseMap = true;
						if (aiscene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &filePath) == AI_SUCCESS)
							texPath[filePath.C_Str()].push_back(i);
					}

					scene->material.push_back(mat);
                }

                // Load textures
                scene->texture.resize(texPath.size());
				UINT j = 0;
				// For every texture
				for (auto const &pair : texPath)
				{
					scene->texture[j] = new Texture();
					// Load texture into scene texture array
					LoadTexture(pair.first, DIFFUSEMAP, scene->texture[j]);

					// Reference the textures correctly in the materials that use them
					for (UINT k = 0; k < pair.second.size(); k++)
					{
						scene->material[pair.second[k]]->diffuseMap = scene->texture[j];
					}

					j++;
				}
            }

            // Load Meshes
            if (aiscene->HasMeshes())
            {
                for (UINT i = 0; i < aiscene->mNumMeshes; i++)
                {
                    Mesh mesh;
                    mesh.name = aiscene->mMeshes[i]->mName.C_Str();

                    // Material information is already loaded in the mesh just needs some direction to it
                    mesh.mat = scene->material[aiscene->mMeshes[i]->mMaterialIndex];

                    // Size the arrays
                    std::vector<VertexNormal> vertices = std::vector<VertexNormal>(aiscene->mMeshes[i]->mNumVertices);

                    // Get the vertices, vertex normals 
                    for (UINT j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
                    {
                        // Vertices
                        vertices[j].x = aiscene->mMeshes[i]->mVertices[j].x;
                        vertices[j].y = aiscene->mMeshes[i]->mVertices[j].y;
                        vertices[j].z = aiscene->mMeshes[i]->mVertices[j].z;
                        vertices[j].nx = aiscene->mMeshes[i]->mNormals[j].x;
                        vertices[j].ny = aiscene->mMeshes[i]->mNormals[j].y;
                        vertices[j].nz = aiscene->mMeshes[i]->mNormals[j].z;
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

                    mesh.SetVertexBuffer(vertices);
                    mesh.SetIndexBuffer(indices);
                    mesh.model = Utility::aiMat4x4ToGLM(aiscene->mRootNode->mChildren[0]->mTransformation);
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
	static bool LoadTexture(std::string filePath, TextureType texType, Texture* texture)
	{
        bool results = false;
        // Remove all the preceding text in the texture filePath
        int pos = filePath.find_last_of(static_cast<unsigned char>(47)) + 1;
        filePath = filePath.substr(pos, filePath.size() - pos);
        std::string name = filePath.substr(0, filePath.length() - 4);

        // If the texture doesn't already exist
	    filePath = "Content/Textures/" + filePath;

	    // Load the png using LodePng
	    std::vector<unsigned char> image;
	    UINT width, height;
	    UINT error = lodepng::decode(image, width, height, filePath);
	    //std::string errorStr = lodepng_error_text(error);

	    if (error == 0)
	    {
		    // Parse the image so I can use it as a pointer
		    texture->buffer = new unsigned char[width * height * 3];
		    for (UINT y = 0; y < height; y++)
		    {
			    for (UINT x = 0; x < width; x++)
			    {
				    UINT i = y * width + x;
				    UINT j = i * 3;
				    UINT k = i * 4;
					texture->buffer[j] = image[k];
					texture->buffer[j + 1] = image[k + 1];
					texture->buffer[j + 2] = image[k + 2];
			    }
		    }

		    // Add the texture to the scene
			texture->name = name;
			texture->width = width;
			texture->height = height;
			texture->type = texType;
		    results = true;
	    }

        return results;
	}
};