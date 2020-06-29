#pragma once
#include "MapperComponent.h"
#include "MathHelper.h"
#include <memory>

class PolyData;
class PolyDataMapper;
class PolyDataTriangulate;

// Depends on PolySource. Implements PolyDataMapper and PolyDataTriangulate
// to display a connected 2d polygon as a solid
class PolyRenderer2d : public MapperComponent
{
public:
	PolyRenderer2d();

public:
	std::shared_ptr<PolyDataMapper> getPolyMapper() const { return polyMapper; }

	void setInput(std::shared_ptr<PolyData> polyData);
	void setTransform(const glm::mat4& transform);

	void update() override;

private:
	std::shared_ptr<PolyDataMapper> polyMapper = nullptr;
	std::shared_ptr<PolyDataTriangulate> triangulatePoly = nullptr;
};