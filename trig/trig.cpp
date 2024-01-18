#include <stdio.h>

#include <cmath>
#include "trig.h"

float fold_sin_input(float x)
{
	float input = x;
#ifdef _DEBUG
	printf(__func__);
	printf("\n");
	printf("input = %lf (%lf)\n", x, x*radToDeg);
#endif

	float sign = 1.0f;

	if (x < 0)
	{
		sign = -1.0f;
		x = -x;
	}

#ifdef _DEBUG
	printf("wrap by 2pi:\n");
#endif

	float div = x * invTwoPi;

	float frac = div - floor(div);

	x = sign * frac * twoPi;

#ifdef _DEBUG
	printf("x = %lf (%lf)\n", x, x*radToDeg);
	printf("sin(x) = %lf, expected = %lf\n", sin(x), sin(input));

	printf("wrap if abs(x) > 3pi/2:\n");
#endif

	if (x > 1.5f*pi)
	{
		x -= twoPi;
	}
	else if (x < -1.5f*pi)
	{
		x += twoPi;
	}

#ifdef _DEBUG
	printf("x = %lf (%lf)\n", x, x * radToDeg);
	printf("sin(x) = %lf, expected = %lf\n", sin(x), sin(input));
	
	printf("wrap if abs(x) > pi/2:\n");
#endif

	if (x > 0.5f * pi)
	{
		x = pi - x;
	}
	else if (x < -0.5f * pi)
	{
		x = -pi - x;
	}

#ifdef _DEBUG
	printf("x = %lf (%lf)\n", x, x * radToDeg);
	printf("sin(x) = %lf, expected = %lf\n", sin(x), sin(input));
#endif
	
	return x;
}

float sin_poly3(float x)
{
	return sin_poly3_safe(fold_sin_input(x));
}

float sin_poly3_safe(float x)
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
	return sin_poly5_safe(fold_sin_input(x));
}

float sin_poly5_safe(float x)
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
