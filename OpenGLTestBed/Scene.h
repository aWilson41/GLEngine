#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "MathHelper.h"

class Scene
{
private:  
   std::string rootPath;

public:
   Camera cam;

   // Resources
   GLuint vaoID;
   std::vector<Mesh> mesh;
   std::vector<Material> material;
   std::vector<Texture> texture;

   glm::vec3 dirLight;
   GLuint dirLightID;

   Scene();

   int FindTexture(std::string name);
   void cleanup();
};