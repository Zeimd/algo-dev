
#include <stdio.h>
#include "exp_log.h"



float pow2_poly3(float x)
{
	FloatInfo info = GetFloatInfo(x);

	float y = 1.0f + float(info.mantissa) * pow2_n24;

	float base = pow2_principal_poly3(y);
	return 0.0f;
}

float pow2_principal_poly1(float x)
{
	const float a = 1.9842f;
	const float b = 1.8933f;

	return a * x + b;
}

float pow2_principal_poly2(float x)
{
	const float a = 0.68531f;
	const float b = 1.2989f;
	const float c = 2.0075f;

	return x * (a * x + b) + c;
}

float pow2_principal_poly3(float x)
{
	const float a = 0.15804;
	const float b = 0.44825f;
	const float c = 1.3937f;
	const float d = 1.9996f;

	return x * (x * (a * x + b) + c) + d;
}

float log2_poly3(float x)
{
	FloatInfo info = GetFloatInfo(x);

	float mantissaF = float(info.mantissa);
	float y = mantissaF * pow2_n24;

	/*
	if (info.exponent < 0)
	{
		y = 1.0f - y;
	}
	*/
	
	

	float base = log2_principal_poly3(y);
	float expF = float(info.exponent);

	float result = expF + base;
	
	/*
	float result;
	if (info.exponent >= 0)
	{
		result = expF + base;
	}
	else
	{
		result = expF - base;
	}
	*/
	
		

#ifdef _DEBUG
	printf("log2_poly3\n");

	printf("input = %lf\n", x);
	FloatPrintBinary("input binary = ", x);
	printf("sign = %i, exp = %i, mantissa = %i\n", info.sign, info.exponent, info.mantissa);
	printf("mantissaF = %lf\n", mantissaF);
	printf("y = %.15lf\n", y);
	printf("expF = %lf\n", expF);
	float correctBase = log2f(1.0f + y);
	float baseAbsErr = fabsf(correctBase - base);

	result = expF + correctBase;

	printf("base = %lf, correct = %lf (absErr = %lf)\n", base, correctBase,baseAbsErr);
	printf("result = %lf\n", result);
#endif

	return result;
}

float log2_principal_poly1(float x)
{
	const float a = 0.98426f;
	const float b = 0.065169f;

	return a * x + b;
}

float log2_principal_poly2(float x)
{
	const float a = -0.33688f;
	const float b = 1.3211f;
	const float c = 0.0090275f;

	return x * (a * x + b) + c;
}

float log2_principal_poly3(float x)
{
	const float a = 0.15391f;
	const float b = -0.56776f;
	const float c = 1.4135f;
	const float d = 0.0013341f;

	return x * (x * (a * x + b) + c) + d;
}