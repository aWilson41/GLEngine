#pragma once
#include "PolyDataMapper.h"
#include "Types.h"

// Does rendering and pre rendering operations (mapping input to visual representation)
// This class does instanced drawing with the input and offset data provided
class GlyphPolyDataMapper : public PolyDataMapper
{
public:
	GlyphPolyDataMapper();
	~GlyphPolyDataMapper();

public:
	GLfloat* getOffsetData() const { return offsetData; }
	GLfloat* getColorData() const { return colorData; }
	UINT getInstanceCount() const { return instanceCount; }
	std::string getMapperName() const override { return "GlyphPolyDataMapper"; }

	void allocateOffsets(UINT count)
	{
		instanceCount = count;
		if (offsetData != nullptr)
			delete[] offsetData;
		offsetData = new GLfloat[count * 3];
	}
	void allocateColorData(UINT count)
	{
		instanceCount = count;
		if (colorData != nullptr)
			delete[] colorData;
		colorData = new GLfloat[count * 3];
	}

	// Updates the buffer based on the set poly data. If it's the first time it initializes the buffer
	void update() override;

	void useShader(std::string shaderGroup) override;
	void draw(Renderer* ren) const override;

protected:
	void updateInfo();
	void updateBuffer();

protected:
	GLuint instanceCount = 0;
	GLfloat* offsetData = nullptr;
	GLfloat* colorData = nullptr;
};