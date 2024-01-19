#pragma once

// 2^-24
constexpr float pow2_n24 = float(1.0 / double(1 << 24));

// 2^-23
constexpr float pow2_n23 = float(1.0 / double(1 << 23));

const uint32_t floatSignMask = uint32_t(1) << 31;
const uint32_t floatExpMask = uint32_t(255) << 23;
const uint32_t floatAbsMask = ~floatSignMask;

const uint32_t floatMantissaMask = (1 << 23) - 1;

struct FloatInfo
{
	uint32_t sign;
	int32_t exponent;
	uint32_t mantissa;
};

FloatInfo GetFloatInfo(float x);

float MakeFloat(uint32_t sign, int32_t exponent, uint32_t mantissa);

void FloatPrintBinary(const char* text, float x);