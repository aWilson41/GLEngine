#pragma once
#include <GL\glew.h>
#include "Material.h"

struct VertexNormalTex
{
    float x, y, z; // Vertex
    float nx, ny, nz; // Normal
    float s0, t0; // Texcoords
};

struct VertexColor
{
    float x, y, z; // Vertex
    float r, g, b, a; // Color
};

class Mesh
{
public:
   std::vector<VertexColor> vertices;
   std::vector<GLuint> indices;

   std::string name;

   // Vertex, Color, Index
   GLuint vertexVboID;
   GLuint indexBufferID;

   matrix4x4 world;

   Material* mat;

public:
   Mesh();
   ~Mesh();

   void SetVertexColorBuffer(std::vector<VertexColor> vertices);
   void SetIndexBuffer(std::vector<GLuint> indices);

public:
   int GetVertexCount() { return vertices.size(); }
};