
#include <stdint.h>

constexpr double piD = 3.14159265358979323846;
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = float(2.0 * piD);

constexpr float invPi = float(1.0 / piD);
constexpr float invTwoPi = float(0.5 / piD);

constexpr float degToRad = float(piD / 180.0);
constexpr float radToDeg = float(180.0 / piD);

typedef float(*SimpleTrigCall)(float x);
typedef float(*TwoParamFoldCall)(float x,float* out_sign);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input(float x);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input_v2(float x, float* out_sign);

float fold_sin_input_v3(float x);

// Valid for any range
float sin_poly3(float x);

// Valid for any range
float sin_poly5(float x);

// Valid for any range
float sin_poly3_v2(float x);

// Valid for any range
float sin_poly5_v2(float x);

// Valid for any range
float sin_poly3_v3(float x);

// Valid for any range
float sin_poly5_v3(float x);

// Valid for any range
float sin_poly3_v1_inline(float x);

// Valid for any range
float sin_poly5_v1_inline(float x);

// Valid for any range
float sin_poly3_v2_inline(float x);

// Valid for any range
float sin_poly5_v2_inline(float x);

// Valid for any range
float sin_poly3_v3_inline(float x);

// Valid for any range
float sin_poly5_v3_inline(float x);

// Only valid for range [-pi/2,pi/2]
float sin_poly3_principal(float x);

// Only valid for range [-pi/2,pi/2]
float sin_poly5_principal(float x);

__forceinline float fold_sin_input_v1_inline(float x)
{
	float input = x;

	float sign = 1.0f;

	if (x < 0)
	{
		sign = -1.0f;
		x = -x;
	}

	float div = x * invTwoPi;

	float frac = div - floor(div);

	float x1 = sign * frac * twoPi;

	float x2 = x1;

	if (x2 > 1.5f * pi)
	{
		x2 = x2 - twoPi;
	}
	else if (x2 < -1.5f * pi)
	{
		x2 = x2 + twoPi;
	}

	float x3 = x2;

	if (x3 > 0.5f * pi)
	{
		x3 = pi - x3;
	}
	else if (x3 < -0.5f * pi)
	{
		x3 = -pi - x3;
	}

	return x3;
}

__forceinline float fold_sin_input_v2_inline(float x, float* out_sign)
{
	float input = x;

	float sign = 1.0f;

	if (x < 0)
	{
		sign = -1.0f;
		x = -x;
	}

	float div = x * invTwoPi;

	float frac = div - floor(div);

	float x1 = frac * twoPi;

	float x2 = x1;

	if (x1 > 1.5f * pi)
	{
		x2 = x1 - twoPi;
	}

	float x3 = x2;

	if (x2 > 0.5f * pi)
	{
		x3 = pi - x2;
	}

	* out_sign = sign;

	return x3;
}


constexpr uint32_t floatSignMask = uint32_t(1) << 31;

__forceinline float fold_sin_input_v3_inline(float x)
{
	float input = x;

	uint32_t* xPtr = (uint32_t*)&x;

	uint32_t sign = *xPtr & floatSignMask;

	// x = abs(x)
	*xPtr &= ~floatSignMask;

	float div = x * invTwoPi;

	float frac = div - floor(div);

	float x1 = frac * twoPi;

	xPtr = (uint32_t*)&x1;

	*xPtr |= sign;

	float x2 = x1;

	if (fabsf(x2) > 1.5f * pi)
	{
		xPtr = (uint32_t*)&x2;
		sign = *xPtr & floatSignMask;

		float temp = -twoPi;
		xPtr = (uint32_t*)&temp;
		*xPtr ^= sign;

		x2 += temp;
	}

	float x3 = x2;

	if (fabsf(x3) > 0.5f * pi)
	{
		xPtr = (uint32_t*)&x3;
		sign = *xPtr & floatSignMask;

		float temp = pi;
		xPtr = (uint32_t*)&temp;
		*xPtr |= sign;

		x3 = temp - x3;
	}

	return x3;
}