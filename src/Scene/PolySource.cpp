#include "PolySource.h"
#include "PolyData.h"

PolySource::PolySource()
{
	polyData = std::make_shared<PolyData>();
}

void PolySource::setInput(std::shared_ptr<PolyData> polyData)
{
	this->polyData->copy(polyData);
	inputChanged.emit(this->polyData);
}