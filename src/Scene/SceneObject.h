#pragma once
#include "Component.h"
#include "MathHelper.h"
#include <memory>
#include <list>

using SceneObjectList = std::list<std::shared_ptr<SceneObject>>;

class SceneObject
{
public:
	SceneObject() = default;
	virtual ~SceneObject() = default;

public:
	// By default, updates all the components on the object
	virtual void update()
	{
		for (UINT i = 0; i < components.size(); i++)
		{
			components[i]->update();
		}
	}

	// Adds a component of type T
	template<class T>
	std::shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> comp = std::make_shared<T>();
		components.push_back(comp);
		comp->attach(this);
		return comp;
	}

	// Gets the first component of type T on object, currently hacky (will add object factories soon)
	template<class T>
	std::shared_ptr<T> GetComponent() const
	{
		for (UINT i = 0; i < components.size(); i++)
		{
			std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(components[i]);
			if (ptr != nullptr)
				return ptr;
		}
		return nullptr;
	}

	template<class T>
	bool ContainsComponent() { return GetComponent<T>() != nullptr; }

private:
	std::vector<std::shared_ptr<Component>> components;
};