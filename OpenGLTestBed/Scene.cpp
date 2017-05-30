#include "Scene.h"

Scene::Scene()
{
    
}

// Returns index of first occurence texture or -1 if not found
int Scene::FindTexture(std::string name)
{
	for (UINT i = 0; i < texture.size(); i++)
	{
		if (texture[i]->name == name)
			return i;
	}

	return -1;
}

void Scene::draw()
{
    glm::mat4 view = cam.GetView();
    glm::vec3 viewDir = -glm::vec3(view[0][2], view[1][2], view[2][2]);
    glm::mat4 proj = cam.GetProj();

    glUniform3fv(cam.viewDirID, 1, &viewDir[0]);

    for (UINT i = 0; i < mesh.size(); i++)
    {
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(glm::mat3(mesh[i].model))));
        glm::mat4 mvp = proj * view * mesh[i].model;

        glUniformMatrix4fv(mesh[i].mat->matID, 1, GL_FALSE, &mesh[i].mat->mat[0][0]);
        glUniformMatrix4fv(mesh[i].mvpID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix3fv(mesh[i].tInvModelID, 1, GL_FALSE, &normalMatrix[0][0]);

        glBindVertexArray(mesh[i].vaoID);
        glDrawElements(GL_TRIANGLES, mesh[i].indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void Scene::cleanup()
{
	glDisableVertexAttribArray(0);
	for (UINT i = 0; i < mesh.size(); i++)
	{
		glDeleteBuffers(1, &mesh[i].vboID);
		glDeleteBuffers(1, &mesh[i].iboID);
		glDeleteVertexArrays(1, &mesh[i].vaoID);
	}

	// Cleanup the materials
	for (UINT i = 0; i < material.size(); i++)
	{
		delete material[i];
	}

	// Cleanup the textures
	for (UINT i = 0; i < texture.size(); i++)
	{
		delete[] texture[i]->buffer;
		delete texture[i];
	}
}

void Scene::merge(Scene* scene)
{
	// Add the meshes to the scene
	for (UINT i = 0; i < scene->mesh.size(); i++)
	{
		mesh.push_back(scene->mesh[i]);
	}

	// Add the materials to the scene. The references in mesh
	for (UINT i = 0; i < scene->material.size(); i++)
	{
		material.push_back(scene->material[i]);
	}

	// Add the textures to the scene
	for (UINT i = 0; i < scene->texture.size(); i++)
	{
		texture.push_back(scene->texture[i]);
	}
}