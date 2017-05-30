#include "Mesh.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Mesh::SetVertexBuffer(std::vector<VertexNormal> vertices)
{
    // Generate a vertex buffer array
    // we use these when we have vbo's with the same layouts
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

   Mesh::vertices = vertices;
   
   // Generate a vertex buffer
   glGenBuffers(1, &vboID);
   glBindBuffer(GL_ARRAY_BUFFER, vboID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), BUFFER_OFFSET(sizeof(float) * 3));
}

void Mesh::SetIndexBuffer(std::vector<GLuint> indices)
{
   Mesh::indices = indices;

   // Setup the index buffer
   glGenBuffers(1, &iboID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}