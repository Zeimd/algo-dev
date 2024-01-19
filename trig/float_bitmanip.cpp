
#include <stdio.h>
#include <stdint.h>

#include "float_bitmanip.h"

FloatInfo GetFloatInfo(float x)
{
	FloatInfo out;

	uint32_t* hack = (uint32_t*)&x;

	out.sign = (*hack & floatSignMask) >> 31;
	out.exponent = ((*hack & floatExpMask) >> 23) - 127;
	out.mantissa = *hack & floatMantissaMask;

	return out;
}

void FloatPrintBinary(const char* text, float x)
{
	printf("%s", text);

	uint32_t* hack = (uint32_t*)&x;

	for (int k = 31; k >= 0; k--)
	{
		uint32_t bit = ((*hack) >> k) & 1;
		printf("%i", bit);
	}
	printf("\n");
}

float MakeFloat(uint32_t sign, int32_t exponent, uint32_t mantissa)
{
	uint32_t hack = sign << 31;

	hack |= (exponent + 127) << 23;
	hack |= mantissa;

	return *(float*)&hack;
}