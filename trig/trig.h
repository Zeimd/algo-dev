#pragma once

#include <stdint.h>
#include <math.h>

#include "float_bitmanip.h"

constexpr double piD = 3.14159265358979323846;
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = float(2.0 * piD);
constexpr float minusTwoPi = -float(2.0 * piD);

constexpr float oneHalfPi = float(1.5 * piD);
constexpr float halfPi = float(0.5 * piD);

constexpr float invPi = float(1.0 / piD);
constexpr float invTwoPi = float(0.5 / piD);

constexpr float degToRad = float(piD / 180.0);
constexpr float radToDeg = float(180.0 / piD);

typedef float(*SimpleTrigCall)(float x);
typedef float(*TwoParamCall)(float x);
typedef float(*TwoParamFoldCall)(float x,float* out_sign);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input(float x);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input_sse_scalar(float x);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input_v2(float x, float* out_sign);

// Fold input to range [-pi/2,pi/2]
float fold_sin_input_v3(float x);

// Valid for any range
float sin_poly3(float x);

// Valid for any range
float sin_poly5(float x);

// Valid for any range
float sin_poly3_horner(float x);

// Valid for any range
float sin_poly5_horner(float x);

// Valid for any range
float sin_poly3_sse_scalar(float x);

// Valid for any range
float sin_poly5_sse_scalar(float x);

// Valid for any range
float sin_poly3_sse_scalar_inline_fold(float x);

// Valid for any range
float sin_poly5_sse_scalar_inline_fold(float x);


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
// Uses least squares error polynomial fit from Octave
float sin_poly3_principal(float x);

// Only valid for range [-pi/2,pi/2]
// Uses least squares error polynomial fit from Octave
float sin_poly5_principal(float x);

// Valid for any range
float sin_poly3_principal_horner(float x);

// Valid for any range
float sin_poly5_principal_horner(float x);

// Valid for any range
float sin_poly3_principal_horner_sse_scalar(float x);

// Valid for any range
float sin_poly5_principal_horner_sse_scalar(float x);


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

__forceinline float fold_sin_input_sse_scalar_inline(float x)
{
	float result;

#ifdef _DEBUG

	float x1, x2, x3;
	float s0, s1, s2;
#endif


	__asm
	{
		/*
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
		*/

		movss xmm0, x;

		movss xmm1, xmm0;
		movss xmm4, floatSignMask;
		movss xmm5, floatAbsMask;

		andps xmm1, xmm4;
		andps xmm0, xmm5;

		// xmm1 = sign(x)
		// xmm0 = abs(x)

		mulss xmm0, invTwoPi;

		movss xmm2, xmm0;
		cvttps2dq xmm2, xmm2;
		cvtdq2ps xmm2, xmm2;

		subss xmm0, xmm2;

		mulss xmm0, twoPi;

		orps xmm0, xmm1;

		// xmm0 = x1 = sign(x) * frac(x / 2pi) * 2pi

#ifdef _DEBUG
		movss x1, xmm0;
		movss s0, xmm1;
#endif

		/*
		if (fabsf(x2) > 1.5f * pi)
		{
			xPtr = (uint32_t*)&x2;
			sign = *xPtr & floatSignMask;

			float temp = -twoPi;
			xPtr = (uint32_t*)&temp;
			*xPtr ^= sign;

			x2 += temp;
		}
		*/

		movss xmm1, xmm0;
		andps xmm1, xmm4;
		movss xmm2, xmm0;
		andps xmm2, xmm5;

#ifdef _DEBUG
		movss s1, xmm1;
#endif

		// xmm1 = sign(x1)
		// xmm2 = abs(x1)

		movss xmm3, oneHalfPi;
		cmpltss xmm3, xmm2;

		// xmm3 = abs(x1) > 1.5pi 

		movss xmm6, minusTwoPi;
		andps xmm3, xmm6;
		xorps xmm3, xmm1;
		addss xmm0, xmm3;

		// xmm0 = x2

#ifdef _DEBUG
		movss x2, xmm0;
#endif

		/*
		if (fabsf(x3) > 0.5f * pi)
		{
			xPtr = (uint32_t*)&x3;
			sign = *xPtr & floatSignMask;

			float temp = pi;
			xPtr = (uint32_t*)&temp;
			*xPtr |= sign;

			x3 = temp - x3;
		}
		*/

		movss xmm1, xmm0;
		andps xmm1, xmm4;
		movss xmm2, xmm0;
		andps xmm2, xmm5;

#ifdef _DEBUG
		movss s2, xmm1;
#endif
		// xmm1 = sign(x2)
		// xmm2 = abs(x2)

		movss xmm3, halfPi;

		cmpltss xmm3, xmm2;

		// xmm3 = abs(x2) > halfPi

		movss xmm6, pi;

		orps xmm6, xmm1;

		subss xmm6, xmm0;

		// xmm6 = sign(x2)*pi - x2

		// Select either x2 or xmm6 according to
		// comparison result

		andps xmm6, xmm3;
		andnps xmm3, xmm0;
		orps xmm3, xmm6;

#ifdef _DEBUG
		movss x3, xmm3;
#endif


		movss result, xmm3;
	}

	return result;
}