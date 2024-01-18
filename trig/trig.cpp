#include <stdio.h>

#include <cmath>
#include "trig.h"

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

	float x2;

	if (x1 > 1.5f*pi)
	{
		x2 = x1 - twoPi;
	}
	else if (x1 < -1.5f*pi)
	{
		x2 += x1 - twoPi;
	}

	float x3;

	if (x2 > 0.5f * pi)
	{
		x3 = pi - x2;
	}
	else if (x2 < -0.5f * pi)
	{
		x3 = -pi - x2;
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

float fold_sin_input_v2(float x)
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

	if (x1 > 1.5f * pi)
	{
		x2 = x1 - twoPi;
	}
	else if (x1 < -1.5f * pi)
	{
		x2 += x1 - twoPi;
	}

	float x3 = x2;

	if (x2 > 0.5f * pi)
	{
		x3 = pi - x2;
	}
	else if (x2 < -0.5f * pi)
	{
		x3 = -pi - x2;
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

float sin_poly5(float x)
{
	return sin_poly5_principal(fold_sin_input(x));
}

float sin_poly5_principal(float x)
{
	const float a = 0.0075741f;
	const float b = 1.9619e-07f;
	const float c = 0.16583f;
	const float d = -3.228e-07;
	const float e = 0.99977;
	const float f = 5.6906e-08;

	float xp2 = x * x;
	float xp3 = x * xp2;
	float xp4 = x * xp3;
	float xp5 = x * xp4;

	return a * xp5 + b * xp4 + c * xp3 + d * xp2 + e * x + f;
}
