#include "ImageDataShiftScale.h"
#include "ImageData.h"
#include "MathHelper.h"
#include "ParallelFor.h"

ImageDataShiftScale::ImageDataShiftScale() { outputData = std::make_shared<ImageData>(); }

template<typename InputType, typename OutputType>
static void shiftScaleCast2(InputType, OutputType, float shift, float scale, std::shared_ptr<ImageData> inputData, std::shared_ptr<ImageData> outputData)
{
	InputType* inputImgPtr = inputData->getData<InputType>();
	OutputType* outputImgPtr = outputData->getData<OutputType>();
	UINT* dim = inputData->getDimensions();
	UINT numPx = dim[0] * dim[1] * dim[2];
	parallelFor(numPx, [&](const UINT& i)
		{
			const float x = static_cast<float>(inputImgPtr[i]);
			outputImgPtr[i] = static_cast<OutputType>((x + shift) * scale);
		});
}
template<typename InputType>
static void shiftScaleCast1(InputType, float shift, float scale, std::shared_ptr<ImageData> inputData, std::shared_ptr<ImageData> outputData)
{
	switch (outputData->getScalarType())
	{
		TemplateMacro(shiftScaleCast2(static_cast<InputType>(0), static_cast<TT>(0), shift, scale, inputData, outputData));
	default:
		printf("Error: Unknown scalar type");
		break;
	}
}

template<typename InputOutputType>
static void shiftScale(InputOutputType, float shift, float scale, std::shared_ptr<ImageData> inputData, std::shared_ptr<ImageData> outputData)
{
	InputOutputType* inputImgPtr = inputData->getData<InputOutputType>();
	InputOutputType* outputImgPtr = outputData->getData<InputOutputType>();
	UINT* dim = inputData->getDimensions();
	UINT numPx = dim[0] * dim[1] * dim[2];

	parallelFor(numPx, [&](const UINT& i)
		{
			outputImgPtr[i] = (inputImgPtr[i] - static_cast<InputOutputType>(shift)) * static_cast<InputOutputType>(scale);
		});
}

void ImageDataShiftScale::update()
{
	outputData->clear();

	if (useOutputType)
	{
		outputData->allocate3DImage(
			inputData->getDimensions(),
			inputData->getSpacing(),
			inputData->getOrigin(),
			inputData->getNumComps(),
			outputType);

		switch (inputData->getScalarType())
		{
			TemplateMacro(shiftScaleCast1(static_cast<TT>(0), shift, scale, inputData, outputData));
		default:
			printf("Error: Unknown scalar type");
			break;
		}
	}
	else
	{
		outputData->allocate3DImage(
			inputData->getDimensions(),
			inputData->getSpacing(),
			inputData->getOrigin(),
			inputData->getNumComps(),
			inputData->getScalarType());

		switch (inputData->getScalarType())
		{
			TemplateMacro(shiftScale(static_cast<TT>(0), shift, scale, inputData, outputData));
		default:
			printf("Error: Unknown scalar type");
			break;
		}
	}
}