#include <stdio.h>

#include <cmath>
#include "trig.h"

float fold_sin_input(float x)
{
	printf(__func__);
	printf("\n");
	printf("input = %lf\n", x);

	float sign = 1.0f;

	if (x < 0)
	{
		sign = -1.0f;
		x = -x;
	}

	printf("wrap by 2pi:\n");

	float div = x * invTwoPi;

	float frac = div - floor(div);

	x = sign * frac * twoPi;

	printf("x = %lf\n", x);

	printf("wrap if abs(x) > 3pi/2:\n");

	if (x > 1.5f*pi)
	{
		x -= pi;
	}
	else if (x < -1.5f*pi)
	{
		x += pi;
	}

	printf("x = %lf\n", x);
	
	printf("wrap if abs(x) > pi/2:\n");

	if (x > 0.5f * pi)
	{
		x = pi - x;
	}
	else if (x < -0.5f * pi)
	{
		x = -pi - x;
	}

	printf("x = %lf\n", x);
	
	return x;
}

float sin_poly3(float x)
{
	return 0.0f;
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
