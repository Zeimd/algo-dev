
#include <limits>

#include "convert.h"

#include "binary_print.h"


uint16_t SingleToHalf(float x)
{
	if (x == std::numeric_limits<float>::quiet_NaN())
	{
		return halfQuietNan;
	}

	if (x == std::numeric_limits<float>::signaling_NaN())
	{
		return halfSignalingNan;
	}

	if (x == std::numeric_limits<float>::infinity())
	{
		return halfPlusInfinity;
	}

	if (x == -1.0f * std::numeric_limits<float>::infinity())
	{
		return halfMinusInfinity;
	}

	uint32_t* punning = (uint32_t*)&x;

	uint32_t sign = *punning & singleSignMask;

	x = fabsf(x);

	if (x > halfMax)
	{
		x = halfMax;
	}

	if (x < halfMin)
	{
		x = halfMin;
	}

	//printf("extracted mantissa: ");
	//PrintBinary(*punning & singleMantissaMask);
	//printf("\n");

	uint32_t mantissa = (*punning & singleMantissaMask) >> 13;

	uint32_t biasedExponent = (*punning & singleExponentMask) >> 23;

	//printf("extracted biased exponent: %i, ", biasedExponent);
	//PrintBinary(biasedExponent);
	//printf("\n");

	int32_t trueExponent = int32_t(biasedExponent) - 127;

	//printf("true exponent: %i, ", trueExponent);
	//PrintBinary(trueExponent);
	//printf("\n");

	sign >>= 16;

	biasedExponent = uint32_t(trueExponent + 15) << 10;

	return sign | mantissa | biasedExponent;	
}

float HalfToSingle(uint16_t x)
{
	return 0.0f;
}