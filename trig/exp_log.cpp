
#include <stdio.h>
#include "exp_log.h"

float exp_poly3(float x)
{
	return exp2_poly3(x * log2e);
}

float exp2_poly3(float x)
{
	FloatInfo info = GetFloatInfo(x);

	float intPart = floor(x);
	float frac = x - intPart;

	float base = MakeFloat(0, intPart, 0);
	
	float princ = exp2_principal_poly3(frac);

#ifdef _DEBUG

	printf("pow2_poly3\n");
	printf("intPart = %lf, frac = %lf\n", intPart, frac);
	printf("base = %lf, princ = %lf\n", base, princ);

#endif

	return base * princ;
}

float exp2_principal_poly1(float x)
{
	const float a = 0.72703f;
	const float b = 1.082f;

	return a * x + b;
}

float exp2_principal_poly2(float x)
{
	const float a = 0.24858f;
	const float b = 0.72703f;
	const float c = 0.99916f;

	return x * (a * x + b) + c;
}

float exp2_principal_poly3(float x)
{
	const float a = 0.057002f;
	const float b = 0.24858f;
	const float c = 0.69282f;
	const float d = 0.99916f;

	return x * (x * (a * x + b) + c) + d;
}

float log2_poly3(float x)
{
	FloatInfo info = GetFloatInfo(x);

	float mantissaF = float(info.mantissa);
	float y = mantissaF * pow2_n23;

	float base = log2_principal_poly3(y);
	float expF = float(info.exponent);

	float result = expF + base;
	
#ifdef _DEBUG
	float fullMantissa = 1.0f + y;

	float backTrack = pow(2.0, float(info.exponent)) * fullMantissa;

	printf("log2_poly3\n");

	printf("input = %lf\n", x);
	FloatPrintBinary("input binary = ", x);
	printf("sign = %i, exp = %i, mantissa = %i\n", info.sign, info.exponent, info.mantissa);
	printf("mantissaF = %lf\n", mantissaF);
	printf("y = %.15lf\n", y);
	printf("fullMantissa = %.15lf\n", fullMantissa);
	printf("backTrack = %.15lf\n", backTrack);
	printf("expF = %lf\n", expF);
	float correctBase = log2f(1.0f + y);
	float baseAbsErr = fabsf(correctBase - base);

	//result = expF + correctBase;

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