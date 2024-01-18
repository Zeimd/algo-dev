#include <stdio.h>

#include <ceng/lib/timerlib.h>
#include <cmath>

#include "trig.h"

void SpeedTests();

double SpeedTest(const char* name, SimpleTrigCall callback, float start, float step, float end, double refDuration);

int FoldTest(const char* name, SimpleTrigCall func, SimpleTrigCall folding, float start, float step, float end);

int FoldTest_v2(const char* name, SimpleTrigCall func, TwoParamFoldCall folding, float start, float step, float end);

int main()
{
	//float x = -269.514191f * degToRad;
	float x = -359.470612f * degToRad;
	
	float sign;
	float folded = fold_sin_input(x);

	float correct = sin(x);
	float testVal = sin(folded);

	//if (fabsf(correct - testVal) > 1.0e-4)
	//{
		printf("input = %lf (%lf)\n", x, x * radToDeg);
		printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
		printf("func(folded) = %lf, expected = %lf\n", testVal, correct);
	//}

	float start = -4.0*pi;
	float end = 4.0*pi;

	float step = 0.01f;

	//FoldTest("sin_folding", &std::sinf, &fold_sin_input, start, step, end);
	//FoldTest("sin_folding_v3", &std::sinf, &fold_sin_input_v3, start, step, end);
	//FoldTest_v2("sin_folding_v2", &std::sinf, &fold_sin_input_v2, start, step, end);

	SpeedTests();

	return 0;
}

int FoldTest_v2(const char* name, SimpleTrigCall func, TwoParamFoldCall folding, float start, float step, float end)
{
	printf("FoldTest: %s\n", name);
	float x = start;

	int errCount = 0;

	while (x < end)
	{
		float sign;
		float folded = folding(x,&sign);

		float correct = func(x);
		float testVal = sign*func(folded);

		if (folded < -0.5f * pi || folded > 0.5f * pi)
		{
			printf("ERROR : input = %lf (%lf)\n", x, x * radToDeg);
			printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
			printf("out or range [-pi/2,pi/2]\n");
			errCount++;
		}
		else if (fabsf(correct - testVal) > 1.0e-3)
		{
			printf("ERROR : input = %lf (%lf)\n", x, x * radToDeg);
			printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
			printf("func(folded) = %lf, expected = %lf\n", testVal, correct);
			errCount++;
		}

		x += step;
	}

	printf("FoldTest: END\n");

	if (errCount)
	{
		printf("Total errors: %i\n", errCount);
	}

	return errCount;
}

int FoldTest(const char* name, SimpleTrigCall func, SimpleTrigCall folding, float start, float step, float end)
{
	printf("FoldTest: %s\n", name);
	float x = start;

	int errCount = 0;

	while (x < end)
	{
		float folded = folding(x);

		float correct = func(x);
		float testVal = func(folded);

		if (folded < -0.5f * pi || folded > 0.5f * pi)
		{
			printf("ERROR : input = %lf (%lf)\n", x, x * radToDeg);
			printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
			printf("out or range [-pi/2,pi/2]\n");
			errCount++;
		}
		else if (fabsf(correct - testVal) > 1.0e-3)
		{
			printf("ERROR : input = %lf (%lf)\n", x, x*radToDeg);
			printf("folded = %lf (%lf)\n", folded, folded*radToDeg);
			printf("func(folded) = %lf, expected = %lf\n", testVal, correct);
			errCount++;
		}

		x += step;
	}

	printf("FoldTest: END\n");

	if (errCount)
	{
		printf("Total errors: %i\n", errCount);
	}

	return errCount;
}

void SpeedTests()
{
	float start = -pi * 4.0;
	float end = pi * 4.0;
	float step = 0.000001f;

	int steps = (end - start) / step;

	printf("steps = %i\n", steps);

	double refDuration = SpeedTest("library sine", &std::sinf, start, step, end, 1.0f);

	SpeedTest("sine poly 3 principal", &sin_poly3_principal, start, step, end, refDuration);
	SpeedTest("sine poly 3", &sin_poly3, start, step, end, refDuration);
	SpeedTest("sine poly 3 v2", &sin_poly3_v2, start, step, end, refDuration);
	SpeedTest("sine poly 3 v3", &sin_poly3_v3, start, step, end, refDuration);

	SpeedTest("sine poly 5 principal", &sin_poly5_principal, start, step, end, refDuration);
	SpeedTest("sine poly 5", &sin_poly5, start, step, end, refDuration);
	SpeedTest("sine poly 5 v2", &sin_poly5_v2, start, step, end, refDuration);
	SpeedTest("sine poly 5 v3", &sin_poly5_v3, start, step, end, refDuration);
}

double SpeedTest(const char* name, SimpleTrigCall callback, float start, float step, float end, double refDuration)
{
	printf("---------------------------------------------------------\n");
	printf("SpeedTest : %s\n", name);

	double time_start = Ceng_HighPrecisionTimer();

	float x = start;

	float total = 0.0f;

	while(x < end)
	{
		float res = (*callback)(x);

		total += res;

		x += step;
	}

	double time_end = Ceng_HighPrecisionTimer();

	double duration = time_end - time_start;

	printf("END\n");
	printf("total = %lf\n", total);
	printf("Duration = %lf , ratio = %lf\n", duration, duration / refDuration);

	return duration;
}