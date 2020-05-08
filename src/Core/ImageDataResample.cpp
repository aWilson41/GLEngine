#include "ImageDataResample.h"
#include "ImageData.h"
#include "MathHelper.h"
#include "ParallelFor.h"

static UINT calcIndex(UINT x, UINT y, UINT width) { return x + y * width; }

template<typename ScalarType>
static ScalarType bilinearSample(const glm::vec2& pos, ScalarType* imgPtr, const glm::uivec2& dim)
{
	const UINT x1 = static_cast<UINT>(pos.x);
	const UINT y1 = static_cast<UINT>(pos.y);
	const UINT x2 = static_cast<UINT>(glm::clamp(static_cast<int>(x1) + 1, 0, static_cast<int>(dim.x) - 1));
	const UINT y2 = static_cast<UINT>(glm::clamp(static_cast<int>(y1) + 1, 0, static_cast<int>(dim.y) - 1));

	const UINT botLeftIndex = calcIndex(x1, y1, dim.x);
	const UINT topLeftIndex = calcIndex(x1, y2, dim.x);
	const UINT topRightIndex = calcIndex(x2, y2, dim.x);
	const UINT botRightIndex = calcIndex(x2, y1, dim.x);

	const ScalarType botLeftVal = imgPtr[botLeftIndex];
	const ScalarType topLeftVal = imgPtr[topLeftIndex];
	const ScalarType topRightVal = imgPtr[topRightIndex];
	const ScalarType botRightVal = imgPtr[botRightIndex];

	const glm::vec2 t = (glm::vec2(static_cast<float>(x2), static_cast<float>(y2)) - pos);

	/*const float topXVal = topLeftVal + t.x * (topRightVal - topLeftVal);
	const float botXVal = botLeftVal + t.x * (botRightVal - botLeftVal);
	return botXVal + t.y * (topXVal - botXVal);*/
	const ScalarType topXVal = topRightVal + t.x * (topLeftVal - topRightVal);
	const ScalarType botXVal = botRightVal + t.x * (botLeftVal - botRightVal);
	return topXVal + t.y * (botXVal - topXVal);
}

template<typename ScalarType>
static void resample(ScalarType* inputImgPtr, ScalarType* outputImgPtr,
	const glm::uivec2& inputDim, const glm::uivec2& outputDim)
{
	// Size between center of pixels
	const glm::vec2 ratio = (glm::vec2(inputDim - glm::uivec2(1))) / glm::vec2(outputDim - glm::uivec2(1));
	parallelFor(outputDim.y, [&](const UINT& y)
		{
			UINT i = y * outputDim.x;
			for (UINT x = 0; x < outputDim.x; x++, i++)
			{
				// Intermediate/coordinate space, coordinate space unmultiplied and offset by spacing
				const glm::vec2 inputPos = glm::vec2(x, y) * ratio;
				outputImgPtr[i] = bilinearSample(inputPos, inputImgPtr, inputDim);
			}
		});
}

ImageDataResample::ImageDataResample() { outputData = std::make_shared<ImageData>(); }

void ImageDataResample::update()
{
	if (inputData->getDimensions()[2] != 1)
	{
		printf("ImageDataResample only supports 2d images");
		return;
	}
	if (inputData->getNumComps() != 1)
	{
		printf("ImageDataResample only supports 1 component images");
		return;
	}

	const glm::uivec2 inputDim = glm::uivec2(inputData->getDimensions()[0], inputData->getDimensions()[1]);
	const glm::uivec2 outputDim = dim;

	const glm::vec2 inputSpacing = glm::vec2(inputData->getSpacing()[0], inputData->getSpacing()[1]);
	const glm::vec2 inputSize = glm::vec2(inputDim) * inputSpacing;
	const glm::vec2 outputSpacing = inputSize / glm::vec2(outputDim);

	UINT outputDimU[2] = { outputDim.x, outputDim.y };
	double outputSpacingD[2] = { static_cast<double>(outputSpacing.x), static_cast<double>(outputSpacing.y) };

	outputData->clear();
	outputData->allocate2DImage(
		outputDimU,
		outputSpacingD,
		inputData->getOrigin(),
		1,
		inputData->getScalarType());

	switch (inputData->getScalarType())
	{
		TemplateMacro(resample(
			inputData->getData<TT>(), outputData->getData<TT>(),
			inputDim, outputDim));
	default:
		break;
	}
}