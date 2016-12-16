#pragma once
#include <GL\glew.h>
#include "Material.h"

class Mesh
{
protected:
   std::vector<float3> vertices;
   std::vector<float3> vertexNormals;
   std::vector<float2> texCoords;
   std::vector<UINT> indices;

public:
   std::string name;

   GLuint vboID;

   matrix4x4 world;

   Material* mat;

public:
   Mesh();
   ~Mesh();

   void SetVertexBuffer(std::vector<float3> vertices, std::vector<float3> vertexNormals, std::vector<float2> texCoords);
   void SetIndexBuffer(std::vector<UINT> indices);

public:
   int GetVertexCount() { return vertices.size(); }
};