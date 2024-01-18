#include <stdio.h>

#include <ceng/lib/timerlib.h>
#include <cmath>

#include "trig.h"

void SpeedTests();

double SpeedTest(const char* name, SimpleTrigCall callback, float start, float step, float end, double refDuration);

int main()
{
	float x = degToRad * 192.0f;

	float y = fold_sin_input(x);

	printf("x = %lf, y = %lf\n", x, y);	
	printf("sin(x) = %lf, sin(y) = %lf\n", sin(x), sin(y));

	return 0;
}

void SpeedTests()
{
	float start = -pi * 0.5;
	float end = pi * 0.5;
	float step = 0.000001f;

	int steps = (end - start) / step;

	printf("steps = %i\n", steps);

	double refDuration = SpeedTest("library sine", &std::sinf, start, step, end, 1.0f);

	SpeedTest("sine poly 3 safe", &sin_poly3_safe, start, step, end, 1.0f);
	SpeedTest("sine poly 5 safe", &sin_poly5_safe, start, step, end, 1.0f);
}

double SpeedTest(const char* name, SimpleTrigCall callback, float start, float step, float end, double refDuration)
{
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