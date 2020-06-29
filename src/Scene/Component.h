#pragma once

class SceneObject;

// Abstract class for components set on scene objects
// Doubly linked
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

public:
	SceneObject* getSceneObject() const { return sceneObject; }

	void attach(SceneObject* sceneObject) { this->sceneObject = sceneObject; }

	virtual void update() { }

protected:
	SceneObject* sceneObject = nullptr;
	bool isInitialized = false;
};