#pragma once

enum class ScalarType
{
	UCHAR_T = 0,
	INT_T = 1,
	FLOAT_T = 2,
	DOUBLE_T = 3
};

enum class CellType
{
	POINT = 0,
	LINE = 1,
	TRIANGLE = 2,
	QUAD = 3
};

	// Typedef the call with the type
#define TemplateMacroCase(typeN, type, call) \
	case typeN: { typedef type TT; call; }; break
#define TemplateMacro(call) \
	TemplateMacroCase(ScalarType::UCHAR_T, unsigned char, call); \
	TemplateMacroCase(ScalarType::INT_T, int, call); \
	TemplateMacroCase(ScalarType::FLOAT_T, float, call); \
	TemplateMacroCase(ScalarType::DOUBLE_T, double, call)