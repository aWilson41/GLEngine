#include "PolyRenderer2d.h"
#include "PolyData.h"
#include "PolyDataMapper.h"
#include "PolyDataTriangulate.h"
#include "SceneObject.h"

PolyRenderer2d::PolyRenderer2d()
{
	mapper = polyMapper = std::make_shared<PolyDataMapper>();
	triangulatePoly = std::make_shared<PolyDataTriangulate>();
}

void PolyRenderer2d::setInput(std::shared_ptr<PolyData> polyData)
{
	triangulatePoly->setInput(polyData);
	triangulatePoly->update();
	polyMapper->setInput(triangulatePoly->getOutput());
	polyMapper->update();
}

void PolyRenderer2d::setTransform(const glm::mat4& transform)
{
	polyMapper->setModelMatrix(transform);
}

void PolyRenderer2d::update()
{
	triangulatePoly->update();
	polyMapper->update();
}