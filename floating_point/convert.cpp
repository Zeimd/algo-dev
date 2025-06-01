
#include <limits>

#include <cmath>

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

	uint32_t sign = (*punning & singleSignMask) >> 16;

	x = fabsf(x);

	if (x > halfMax)
	{
		x = halfMax;
	}

	if (x < halfNormalMin)
	{
		if (x < halfSubnormalMin)
		{
			return 0;
		}

		//printf("subnormal number\n");

		uint32_t biasedExponent = (*punning & singleExponentMask) >> 23;
		int32_t trueExponent = int32_t(biasedExponent) - 127;

		//printf("trueExponent = %i\n", trueExponent);

		uint32_t mantissa = ((*punning) >> 23) | singleImplicitOne;

		//printf("extracted mantissa with implicit one: ");
		//PrintBinary(mantissa);
		//printf("\n");

		int32_t delta = abs(trueExponent+1);

		//printf("delta = %i\n", delta);

		mantissa >>= delta;

		//printf("normalized mantissa: ");
		//PrintBinary(mantissa);
		//printf("\n");

		return sign | mantissa;
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

	biasedExponent = uint32_t(trueExponent + 15) << 10;

	return sign | mantissa | biasedExponent;	
}

float HalfToSingle(uint16_t x)
{
	if (x == halfQuietNan)
	{
		return std::numeric_limits<float>::quiet_NaN();
	}

	if (x == halfSignalingNan)
	{
		return std::numeric_limits<float>::signaling_NaN();
	}

	if (x == halfPlusInfinity)
	{
		return std::numeric_limits<float>::infinity();
	}

	if (x == halfMinusInfinity)
	{
		return -1.0f * std::numeric_limits<float>::infinity();
	}

	float temp;

	uint32_t* punning = (uint32_t*)&temp;

	uint32_t sign = (x & halfSignMask) << 16;

	uint32_t mantissa = x & halfMantissaMask;

	uint32_t biasedExponent = (x & halfExponentMask) >> 10;

	if (biasedExponent == 0 && mantissa != 0)
	{
		temp = halfNormalMin * float(mantissa) * halfMantissaDiv;

		* punning |= sign;

		return temp;
	}

	mantissa <<= 13;

	int32_t trueExponent = int32_t(biasedExponent) - 15;

	biasedExponent = uint32_t(trueExponent + 127) << 23;

	*punning = sign | mantissa | biasedExponent;

	return temp;
}