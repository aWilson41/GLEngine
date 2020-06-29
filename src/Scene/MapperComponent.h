#pragma once
#include "Component.h"
#include <memory>

class AbstractMapper;

class MapperComponent : public Component
{
protected:
	MapperComponent() = default;
	
public:
	MapperComponent(const MapperComponent&) = delete;
	MapperComponent(const MapperComponent&&) = delete;
	virtual ~MapperComponent() override = default;

public:
	std::shared_ptr<AbstractMapper> getMapper() const { return mapper; }

protected:
	std::shared_ptr<AbstractMapper> mapper = nullptr;
};