#include "Mesh.h"

Mesh::Mesh()
{
   world = MathHelper::MatrixIdentity();
}

void Mesh::SetVertexColorBuffer(std::vector<VertexColor> vertices)
{
   Mesh::vertices = vertices;
   //glGenBuffers(1u, &vboID);
   //glBindBuffer(GL_ARRAY_BUFFER, vboID); 
   //glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices, GL_STATIC_DRAW);
   //glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
}

void Mesh::SetIndexBuffer(std::vector<GLuint> indices)
{
   Mesh::indices = indices;
}

Mesh::~Mesh()
{
   //glDeleteBuffers(vertices.size() * 3 * sizeof(float), &vboID);
}