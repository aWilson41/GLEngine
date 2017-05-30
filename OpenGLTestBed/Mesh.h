#pragma once
#include "Material.h"
#include "MathHelper.h"

#include <GL\glew.h>

struct VertexNormalTex
{
    float x, y, z; // Vertex
    float nx, ny, nz; // Normal
    float s0, t0; // Texcoords
};

struct VertexNormal
{
    float x, y, z; // Vertex
    float nx, ny, nz; // Normal
};

struct VertexColor
{
    float x, y, z; // Vertex
    float r, g, b, a; // Color
};

class Mesh
{
public:
   std::string name;

   // Vertex, Color, Index
   GLuint vaoID;
   GLuint vboID;
   GLuint iboID;
   GLuint mvpID;
   GLuint tInvModelID;

   std::vector<VertexNormal> vertices;
   std::vector<GLuint> indices;
   glm::mat4 model = glm::mat4();;

   Material* mat;

public:
   void SetVertexBuffer(std::vector<VertexNormal> vertices);
   void SetIndexBuffer(std::vector<GLuint> indices);

   int GetVertexCount() { return vertices.size(); }
};