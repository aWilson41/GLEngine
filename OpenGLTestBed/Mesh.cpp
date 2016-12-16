#include "Mesh.h"

Mesh::Mesh()
{
   world = MathHelper::MatrixIdentity();
}

void Mesh::SetVertexBuffer(std::vector<float3> vertices, std::vector<float3> vertexNormals, std::vector<float2> texCoords)
{
   Mesh::vertices = vertices;
   Mesh::vertexNormals = vertexNormals;
   Mesh::texCoords = texCoords;
   //glGenBuffers(1u, &vboID);
   //glBindBuffer(GL_ARRAY_BUFFER, vboID); 
   //glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices, GL_STATIC_DRAW);
   //glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
}

void Mesh::SetIndexBuffer(std::vector<UINT> indices)
{
   Mesh::indices = indices;
}

Mesh::~Mesh()
{
   //glDeleteBuffers(vertices.size() * 3 * sizeof(float), &vboID);
}