#include "ImageData.h"

void ImageData::allocate2DImage(UINT* dim, double* spacing, double* origin, UINT numComps, ScalarType type)
{
	this->dim[0] = dim[0];
	this->dim[1] = dim[1];
	this->dim[2] = 1;
	this->spacing[0] = spacing[0];
	this->spacing[1] = spacing[1];
	this->spacing[2] = spacing[2];
	this->origin[0] = origin[0];
	this->origin[1] = origin[1];
	this->origin[2] = 0.0;
	this->numComps = numComps;
	this->type = type;
	updateBounds();

	const UINT count = dim[0] * dim[1] * numComps;
	switch (type)
	{
		TemplateMacro(allocateData<TT>(count));
	default:
		break;
	}
}
void ImageData::allocate3DImage(UINT* dim, double* spacing, double* origin, UINT numComps, ScalarType type)
{
	this->dim[0] = dim[0];
	this->dim[1] = dim[1];
	this->dim[2] = dim[2];
	this->spacing[0] = spacing[0];
	this->spacing[1] = spacing[1];
	this->spacing[2] = spacing[2];
	this->origin[0] = origin[0];
	this->origin[1] = origin[1];
	this->origin[2] = origin[2];
	this->numComps = numComps;
	this->type = type;
	updateBounds();

	const UINT count = dim[0] * dim[1] * dim[2] * numComps;
	switch (type)
	{
		TemplateMacro(allocateData<TT>(count));
	default:
		break;
	}
}

void ImageData::updateBounds()
{
	bounds[0] = origin[0];
	bounds[1] = origin[0] + dim[0] * spacing[0];
	bounds[2] = origin[1];
	bounds[3] = origin[1] + dim[1] * spacing[1];
	bounds[4] = origin[2];
	bounds[5] = origin[2] + dim[2] * spacing[2];
}

void ImageData::clear()
{
	const UINT count = dim[0] * dim[1] * dim[2] * numComps;
	switch (type)
	{
		TemplateMacro(clearData<TT>(count));
	default:
		break;
	}
}