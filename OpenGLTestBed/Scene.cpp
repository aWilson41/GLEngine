#include "Scene.h"

Scene::Scene()
{
    // Generate a vertex buffer array
    // we use these when we have vbo's with the same layouts
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
}

// Returns index of first occurence texture or -1 if not found
int Scene::FindTexture(std::string name)
{
	for (UINT i = 0; i < texture.size(); i++)
	{
		if (texture[i].name == name)
			return i;
	}

	return -1;
}

void Scene::cleanup()
{
    glDisableVertexAttribArray(0);
    for (UINT i = 0; i < mesh.size(); i++)
    {
        glDeleteBuffers(1, &mesh[i].vboID);
    }

    glDeleteVertexArrays(1, &vaoID);

    // Cleanup the textures
    for (UINT i = 0; i < texture.size(); i++)
    {
        delete[] texture[i].buffer;
    }
}