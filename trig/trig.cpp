#include <stdio.h>

#include <cmath>
#include "trig.h"

#include <stdint.h>

float fold_sin_input(float x)
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

	if (x2 > 1.5f*pi)
	{
		x2 = x2 - twoPi;
	}
	else if (x2 < -1.5f*pi)
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

#ifdef _DEBUG
	printf(__func__);
	printf("\n");
	printf("input = %lf (%lf)\n", x, x * radToDeg);

	printf("wrap by 2pi:\n");

	printf("x1 = %lf (%lf)\n", x1, x1 * radToDeg);
	printf("sin(x1) = %lf, expected = %lf\n", sin(x1), sin(input));

	printf("wrap if abs(x) > 3pi/2:\n");

	printf("x2 = %lf (%lf)\n", x2, x2 * radToDeg);
	printf("sin(x2) = %lf, expected = %lf\n", sin(x2), sin(input));

	printf("wrap if abs(x) > pi/2:\n");

	printf("x3 = %lf (%lf)\n", x3, x3 * radToDeg);
	printf("sin(x3) = %lf, expected = %lf\n", sin(x3), sin(input));
#endif
	
	return x3;
}

float fold_sin_input_sse_scalar(float x)
{
	float result;

	float x1, x2, x3;

	float s0, s1, s2;

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
		cmpltps xmm3, xmm2;

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

#ifdef _DEBUG
	printf(__func__);
	printf("\n");
	printf("input = %lf (%lf)\n", x, x* radToDeg);
	printf("sign(x) = %lf\n", s0);

	printf("wrap by 2pi:\n");

	printf("x1 = %lf (%lf)\n", x1, x1* radToDeg);
	printf("sign(x1) = %lf\n", s1);
	printf("sin(x1) = %lf, expected = %lf\n", sin(x1), sin(x));

	printf("wrap if abs(x) > 3pi/2:\n");

	printf("x2 = %lf (%lf)\n", x2, x2* radToDeg);
	printf("sign(x2) = %lf\n", s2);
	printf("sin(x2) = %lf, expected = %lf\n", sin(x2), sin(x));
	printf("wrap if abs(x) > pi/2:\n");

	printf("x3 = %lf (%lf)\n", x3, x3* radToDeg);
	printf("sin(x3) = %lf, expected = %lf\n", sin(x3), sin(x));
#endif
	return result;
}

float fold_sin_input_v2(float x, float* out_sign)
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

#ifdef _DEBUG
	printf(__func__);
	printf("\n");
	printf("input = %lf (%lf)\n", x, x * radToDeg);

	printf("sign = %lf\n", sign);

	printf("wrap by 2pi:\n");

	printf("x1 = %lf (%lf)\n", x1, x1 * radToDeg);
	printf("sin(x1) = %lf, expected = %lf\n", sin(x1), sin(input));

	printf("wrap if abs(x) > 3pi/2:\n");

	printf("x2 = %lf (%lf)\n", x2, x2 * radToDeg);
	printf("sin(x2) = %lf, expected = %lf\n", sin(x2), sin(input));

	printf("wrap if abs(x) > pi/2:\n");

	printf("x3 = %lf (%lf)\n", x3, x3 * radToDeg);
	printf("sin(x3) = %lf, expected = %lf\n", sin(x3), sin(input));
#endif

	* out_sign = sign;

	return x3;
}

float fold_sin_input_v3(float x)
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

#ifdef _DEBUG
	printf(__func__);
	printf("\n");
	printf("input = %lf (%lf)\n", x, x * radToDeg);

	printf("wrap by 2pi:\n");

	printf("x1 = %lf (%lf)\n", x1, x1 * radToDeg);
	printf("sin(x1) = %lf, expected = %lf\n", sin(x1), sin(input));

	printf("wrap if abs(x) > 3pi/2:\n");

	printf("x2 = %lf (%lf)\n", x2, x2 * radToDeg);
	printf("sin(x2) = %lf, expected = %lf\n", sin(x2), sin(input));

	printf("wrap if abs(x) > pi/2:\n");

	printf("x3 = %lf (%lf)\n", x3, x3 * radToDeg);
	printf("sin(x3) = %lf, expected = %lf\n", sin(x3), sin(input));
#endif

	return x3;
}


float sin_poly3(float x)
{
	return sin_poly3_principal(fold_sin_input(x));
}

float sin_poly5(float x)
{
	return sin_poly5_principal(fold_sin_input(x));
}

float sin_poly3_horner(float x)
{
	return sin_poly3_principal_horner(fold_sin_input(x));
}

float sin_poly5_horner(float x)
{
	return sin_poly5_principal_horner(fold_sin_input(x));
}

float sin_poly3_sse_scalar(float x)
{
	return sin_poly3_principal(fold_sin_input_sse_scalar(x));
}

float sin_poly5_sse_scalar(float x)
{
	return sin_poly5_principal(fold_sin_input_sse_scalar(x));
}

float sin_poly3_v2(float x)
{
	float sign;
	
	float folded = fold_sin_input_v2(x, &sign);

	return sign*sin_poly3_principal(folded);
}

float sin_poly5_v2(float x)
{
	float sign;

	float folded = fold_sin_input_v2(x, &sign);

	return sign * sin_poly5_principal(folded);
}

float sin_poly3_v2_inline(float x)
{
	float sign;

	float folded = fold_sin_input_v2_inline(x, &sign);

	return sign * sin_poly3_principal(folded);
}

float sin_poly5_v2_inline(float x)
{
	float sign;

	float folded = fold_sin_input_v2_inline(x, &sign);

	return sign * sin_poly5_principal(folded);
}

float sin_poly3_v3(float x)
{
	return sin_poly3_principal(fold_sin_input_v3(x));
}

float sin_poly5_v3(float x)
{
	return sin_poly5_principal(fold_sin_input_v3(x));
}

float sin_poly3_v1_inline(float x)
{
	return sin_poly3_principal(fold_sin_input_v1_inline(x));
}

float sin_poly5_v1_inline(float x)
{
	return sin_poly5_principal(fold_sin_input_v1_inline(x));
}

float sin_poly3_v3_inline(float x)
{
	return sin_poly3_principal(fold_sin_input_v3_inline(x));
}

float sin_poly5_v3_inline(float x)
{
	return sin_poly5_principal(fold_sin_input_v3_inline(x));
}

// Valid for any range
float sin_poly3_principal_horner(float x)
{
	const float a = -0.14506f;
	const float b = -5.1833e-06f;
	const float c = 0.98879f;
	const float d = 2.5585e-06;

	return x * (x * (a * x + b) + c) + d;
}

// Valid for any range
float sin_poly5_principal_horner(float x)
{
	const float a = 0.0075741f;
	const float b = 1.9619e-07f;
	const float c = -0.16583f;
	const float d = -3.228e-07;
	const float e = 0.99977;
	const float f = 5.6906e-08;

	return x * (x * (x * (x * (a * x + b) + c) + d) + e) + f;
}

float sin_poly3_principal(float x)
{
	const float a = -0.14506f;
	const float b = -5.1833e-06f;
	const float c = 0.98879f;
	const float d = 2.5585e-06;

	float xp2 = x * x;
	float xp3 = x * xp2;

	return a * xp3 + b * xp2 + c * x + d;
}

float sin_poly5_principal(float x)
{
	const float a = 0.0075741f;
	const float b = 1.9619e-07f;
	const float c = -0.16583f;
	const float d = -3.228e-07;
	const float e = 0.99977;
	const float f = 5.6906e-08;

	float xp2 = x * x;
	float xp3 = x * xp2;
	float xp4 = x * xp3;
	float xp5 = x * xp4;

	return a * xp5 + b * xp4 + c * xp3 + d * xp2 + e * x + f;
}
