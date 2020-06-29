#include "PolyRenderer.h"
#include "PolyData.h"
#include "PolyDataMapper.h"
#include "SceneObject.h"

PolyRenderer::PolyRenderer()
{
	mapper = polyMapper = std::make_shared<PolyDataMapper>();
}

void PolyRenderer::setInput(std::shared_ptr<PolyData> polyData)
{
	polyMapper->setInput(polyData);
	polyMapper->update();
}

void PolyRenderer::setTransform(const glm::mat4& transform)
{
	polyMapper->setModelMatrix(transform);
}

void PolyRenderer::update()
{
	polyMapper->update();
}