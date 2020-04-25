#include "AssimpPolyReader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "PolyData.h"

AssimpPolyReader::AssimpPolyReader()
{
	outputData = std::make_shared<PolyData>();
}

void AssimpPolyReader::update()
{
	if (fileName == "")
	{
		printf("AssimpPolyReader: No file name specified.\n");
		return;
	}

	outputData->clear();

	// Transforms are lost because they are kept in the scene graph
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate);
	if (scene == nullptr)
	{
		printf("AssimpPolyReader: Error reading scene.\n");
		return;
	}
	if (!scene->HasMeshes())
	{
		printf("AssimpPolyReader: Scene has no meshes.\n");
		return;
	}

	const UINT meshIndex = 0;
	aiMesh* mesh = scene->mMeshes[meshIndex];
	aiMaterial* material = scene->mMaterials[0];

	if (!mesh->HasPositions())
	{
		printf("AssimpPolyReader: Mesh has no meshes.\n");
		return;
	}

	// Read the points and normals
	outputData->allocateVertexData(mesh->mNumVertices);
	glm::vec3* outputVertexData = reinterpret_cast<glm::vec3*>(outputData->getVertexData());
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		outputVertexData[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	}

	if (mesh->HasNormals())
	{
		outputData->allocateNormalData();
		glm::vec3* normalData = reinterpret_cast<glm::vec3*>(outputData->getNormalData());
		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			normalData[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
	}
	// Currently only using channel 0
	if (mesh->GetNumUVChannels() > 0)
	{
		if (mesh->mNumUVComponents[0] != 2)
		{
			printf("AssimpPolyReader: Reader only supports 2d texture coordinates.\n");
			return;
		}
		outputData->allocateTexCoords();
		glm::vec2* texCoords = reinterpret_cast<glm::vec2*>(outputData->getTexCoordData());
		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			texCoords[i] = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
	}

	// Read the cells
	if (mesh->HasFaces())
	{
		outputData->allocateIndexData(mesh->mNumFaces * 3, CellType::TRIANGLE);
		UINT* indices = outputData->getIndexData();
		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}
	}
}