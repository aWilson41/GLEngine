#pragma once
#include "Component.h"
#include "Signal.h"
#include <memory>

class PolyData;

// Implements a poly data source for other components to use
class PolySource : public Component
{
public:
	PolySource();

public:
	std::shared_ptr<PolyData> getOutput() const { return polyData; }

	// Copies the data
	void setInput(std::shared_ptr<PolyData> polyData);

public:
	Signal<std::shared_ptr<PolyData>> inputChanged;

protected:
	std::shared_ptr<PolyData> polyData = nullptr;
};