#pragma once
#include "MapperComponent.h"
#include "MathHelper.h"
#include <memory>

class PolyData;
class PolyDataMapper;

class PolyRenderer : public MapperComponent
{
public:
	PolyRenderer();
	virtual ~PolyRenderer() override = default;

public:
	std::shared_ptr<PolyDataMapper> getPolyMapper() const { return polyMapper; }

	void setInput(std::shared_ptr<PolyData> polyData);
	void setTransform(const glm::mat4& transform);

	void update() override;

private:
	std::shared_ptr<PolyDataMapper> polyMapper = nullptr;
};