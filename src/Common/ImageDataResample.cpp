#include "ImageDataResample.h"
#include "ImageData.h"
#include "MathHelper.h"
#include "ParallelFor.h"

static UINT calcIndex(UINT x, UINT y, UINT z, const glm::uivec3& dim) { return x + y * dim.x + z * dim.x * dim.y; }


// This sampling function assumes input pos already in image coordinate space (ie spacing of 1)
template<typename ScalarType>
static ScalarType trilinearSample(const glm::vec3& pos, ScalarType* imgPtr, const glm::uivec3& dim)
{
	const UINT x1 = static_cast<UINT>(pos.x);
	const UINT y1 = static_cast<UINT>(pos.y);
	const UINT z1 = static_cast<UINT>(pos.z);
	const UINT x2 = static_cast<UINT>(glm::clamp(static_cast<int>(x1) + 1, 0, static_cast<int>(dim.x) - 1));
	const UINT y2 = static_cast<UINT>(glm::clamp(static_cast<int>(y1) + 1, 0, static_cast<int>(dim.y) - 1));
	const UINT z2 = static_cast<UINT>(glm::clamp(static_cast<int>(z1) + 1, 0, static_cast<int>(dim.z) - 1));

	const UINT index000 = calcIndex(x1, y1, z1, dim);
	const UINT index100 = calcIndex(x2, y1, z1, dim);
	const UINT index110 = calcIndex(x2, y2, z1, dim);
	const UINT index010 = calcIndex(x1, y2, z1, dim);

	const UINT index001 = calcIndex(x1, y1, z2, dim);
	const UINT index101 = calcIndex(x2, y1, z2, dim);
	const UINT index111 = calcIndex(x2, y2, z2, dim);
	const UINT index011 = calcIndex(x1, y2, z2, dim);

	const ScalarType val000 = imgPtr[index000];
	const ScalarType val100 = imgPtr[index100];
	const ScalarType val110 = imgPtr[index110];
	const ScalarType val010 = imgPtr[index010];

	const ScalarType val001 = imgPtr[index001];
	const ScalarType val101 = imgPtr[index101];
	const ScalarType val111 = imgPtr[index111];
	const ScalarType val011 = imgPtr[index011];

	// Interpolants
	const glm::vec3 t = glm::vec3(static_cast<float>(x2), static_cast<float>(y2), static_cast<float>(z2)) - pos;

	/*const float topXVal = topLeftVal + t.x * (topRightVal - topLeftVal);
	const float botXVal = botLeftVal + t.x * (botRightVal - botLeftVal);
	return botXVal + t.y * (topXVal - botXVal);*/
	//const ScalarType topXVal = topRightVal + t.x * (topLeftVal - topRightVal);
	//const ScalarType botXVal = botRightVal + t.x * (botLeftVal - botRightVal);


	// Interpolate along x
	//ScalarType ax = val000 + (val100 - val000) * t.x;
	//ScalarType bx = val010 + (val110 - val010) * t.x;

	//ScalarType dx = val001 + (val101 - val001) * t.x;
	//ScalarType ex = val011 + (val111 - val011) * t.x;

	//// Interpolate along y
	//ScalarType cy = ax + (bx - ax) * t.y;
	//ScalarType fy = dx + (ex - dx) * t.y;

	//// Interpolate along z
	//ScalarType gz = cy + (fy - cy) * t.z;


	ScalarType ax = val100 + (val000 - val100) * t.x;
	ScalarType bx = val110 + (val010 - val110) * t.x;

	ScalarType dx = val101 + (val001 - val101) * t.x;
	ScalarType ex = val111 + (val011 - val111) * t.x;

	// Interpolate along y
	ScalarType cy = bx + (ax - bx) * t.y;
	ScalarType fy = ex + (dx - ex) * t.y;

	// Interpolate along z
	ScalarType gz = fy + (cy - fy) * t.z;

	return gz;
}

template<typename ScalarType>
static void resample(ScalarType* inputImgPtr, ScalarType* outputImgPtr,
	const glm::uivec3& inputDim, const glm::uivec3& outputDim)
{
	// Size between center of pixels
	const glm::vec3 ratio = (glm::vec3(inputDim - glm::uivec3(1))) / glm::vec3(outputDim - glm::uivec3(1));
	parallelFor(outputDim.z, [&](const UINT& z)
		{
			UINT i = z * outputDim.x * outputDim.y;
			for (UINT y = 0; y < outputDim.y; y++)
			{
				for (UINT x = 0; x < outputDim.x; x++, i++)
				{
					// Intermediate/coordinate space, coordinate space unmultiplied and offset by spacing
					const glm::vec3 inputPos = glm::vec3(x, y, z) * ratio;
					outputImgPtr[i] = trilinearSample(inputPos, inputImgPtr, inputDim);
				}
			}
		});
}

ImageDataResample::ImageDataResample() { outputData = std::make_shared<ImageData>(); }

void ImageDataResample::update()
{
	if (inputData->getNumComps() != 1)
	{
		printf("ImageDataResample only supports 1 component images");
		return;
	}

	const glm::uivec3 inputDim = glm::uivec3(inputData->getDimensions()[0], inputData->getDimensions()[1], inputData->getDimensions()[2]);
	const glm::uivec3 outputDim = dim;

	const glm::vec3 inputSpacing = glm::vec3(inputData->getSpacing()[0], inputData->getSpacing()[1], inputData->getSpacing()[2]);
	const glm::vec3 inputSize = glm::vec3(inputDim) * inputSpacing;
	const glm::vec3 outputSpacing = inputSize / glm::vec3(outputDim);

	UINT outputDimU[3] = { outputDim.x, outputDim.y, outputDim.z };
	double outputSpacingD[3] = { static_cast<double>(outputSpacing.x), static_cast<double>(outputSpacing.y), static_cast<double>(outputSpacing.z) };

	outputData->clear();
	outputData->allocate3DImage(
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