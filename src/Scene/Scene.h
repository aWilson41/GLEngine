#include "MathHelper.h"
#include "PropertyMap.h"
#include <memory>
#include <unordered_set>

class SceneObject;

class Scene
{
public:
	using iterator = std::unordered_set<std::shared_ptr<SceneObject>>::iterator;
	using const_iterator = std::unordered_set<std::shared_ptr<SceneObject>>::const_iterator;

public:
	const glm::vec3& getLightDir() const { return lightDir; }

public:
	virtual void addSceneObject(std::shared_ptr<SceneObject> obj);

	virtual void removeSceneObject(std::shared_ptr<SceneObject> obj);

	iterator begin() { return sceneObjects.begin(); }
	iterator end() { return sceneObjects.end(); }

protected:
	std::unordered_set<std::shared_ptr<SceneObject>> sceneObjects;

	glm::vec3 lightDir = glm::vec3(0.0f, 1.0f, 1.0f); // Temporarily only supporting a single directional light
	PropertyMap<32> sceneProperties;
};