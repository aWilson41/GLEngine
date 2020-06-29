#include "Scene.h"
#include "SceneObject.h"

void Scene::addSceneObject(std::shared_ptr<SceneObject> obj)
{
	sceneObjects.insert(obj);
}

void Scene::removeSceneObject(std::shared_ptr<SceneObject> obj)
{
	sceneObjects.erase(obj);
}