#include <stdio.h>

#include <ceng/lib/timerlib.h>
#include <cmath>

#include <limits>

#include "trig.h"
#include "exp_log.h"

#include <ceng/datatypes/aligned-buffer.h>

void TrigSpeedTests();
void TrigAccuracyTests();

void LogSpeedTests();
void LogAccuracyTests();

int AccuracyTest(const char* name, SimpleTrigCall reference, SimpleTrigCall callback, float* inputData, int inputSize, float targetAccuracy);

double SpeedTest(const char* name, SimpleTrigCall callback, float* inputData, int inputSize, double refDuration);

int FoldTest(const char* name, SimpleTrigCall func, SimpleTrigCall folding, float start, float step, float end);

int FoldTest_v2(const char* name, SimpleTrigCall func, TwoParamFoldCall folding, float start, float step, float end);

int main()
{
	//float x = -269.514191f * degToRad;
	//float x = -359.470612f * degToRad;
	float x = 0.004101f;
	//float x = 1.004101f;
	//float x = 1.0f / 256.0f;

	float correct = log2(x);
	float testVal = log2_poly3(x);
	
	float absErr = fabsf(testVal - correct);

	printf("input = %lf\n", x);
	printf("poly(x) = %lf, expected = %lf\n", testVal, correct);
	printf("absErr = %lf\n", absErr);

	/*
	float sign;
	float folded = fold_sin_input_sse_scalar(x);

	float correct = sin(x);
	float testVal = sin(folded);

	//if (fabsf(correct - testVal) > 1.0e-4)
	//{
		printf("input = %lf (%lf)\n", x, x * radToDeg);
		printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
		printf("func(folded) = %lf, expected = %lf\n", testVal, correct);
	//}
	*/

	float start = -4.0*pi;
	float end = 4.0*pi;

	float step = 0.01f;

	//FoldTest("sin_folding", &std::sinf, &fold_sin_input, start, step, end);
	//FoldTest("sin_folding_v3", &std::sinf, &fold_sin_input_v3, start, step, end);
	//FoldTest("sin_folding_sse_scalar", &std::sinf, &fold_sin_input_sse_scalar, start, step, end);
	//FoldTest_v2("sin_folding_v2", &std::sinf, &fold_sin_input_v2, start, step, end);

	//LogSpeedTests();
	//LogAccuracyTests();
	//TrigSpeedTests();
	//AccuracyTests();

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

	int iters = 0;

	while (x < end)
	{
		float folded = folding(x);

		float correct = func(x);
		float testVal = func(folded);

		if (folded == std::numeric_limits<float>::quiet_NaN() ||
			folded == std::numeric_limits<float>::signaling_NaN())
		{
			if (errCount < 10)
			{
				printf("ERROR : input = %lf (%lf)\n", x, x * radToDeg);
				printf("Folded is NaN\n");
			}
			errCount++;
		}
		else if (folded < -0.5f * pi || folded > 0.5f * pi)
		{
			if (errCount < 10)
			{
				printf("ERROR : input = %lf (%lf)\n", x, x * radToDeg);
				printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
				printf("out or range [-pi/2,pi/2]\n");
			}
			errCount++;
		}
		else if (fabsf(correct - testVal) > 1.0e-3)
		{
			if (errCount < 10)
			{
				printf("ERROR : input = %lf (%lf)\n", x, x * radToDeg);
				printf("folded = %lf (%lf)\n", folded, folded * radToDeg);
				printf("func(folded) = %lf, expected = %lf\n", testVal, correct);
			}
			errCount++;
		}

		if (errCount == 10)
		{
			printf("Too many errors. Prints suppressed.\n");
		}
		

		x += step;
		++iters;
	}

	printf("FoldTest: END\n");

	if (errCount)
	{
		double errorPercent = double(errCount) / double(iters) * 100.0;

		printf("Total errors: %i out of %i (%lf)\n", errCount, iters, errorPercent);
	}

	return errCount;
}

void TrigAccuracyTests()
{
	int groups = 10000000;
	int inputSize = 4 * groups;

	float* inputData = (float*)Ceng::AlignedMalloc(inputSize * sizeof(float), 16);

	for (int k = 0; k < inputSize; k++)
	{
		inputData[k] = rand() * 0.0001f;
	}

	printf("steps = %i\n", inputSize);

	float targetAccuracy = 0.005f;

	//AccuracyTest("sine poly 3 principal", &std::sinf, &sin_poly3_principal, inputData, inputSize, targetAccuracy);
	//AccuracyTest("sine poly 5 principal", &std::sinf, &sin_poly5_principal, inputData, inputSize, targetAccuracy);

	AccuracyTest("sine poly 3", &std::sinf, &sin_poly3, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 3 horner", &std::sinf, &sin_poly3_horner, inputData, inputSize, targetAccuracy);
	/*
	AccuracyTest("sine poly 3 v2", &std::sinf, &sin_poly3_v2, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 3 v3", &std::sinf, &sin_poly3_v3, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 3 v1 inline", &std::sinf, &sin_poly3_v1_inline, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 3 v2 inline", &std::sinf, &sin_poly3_v2_inline, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 3 v3 inline", &std::sinf, &sin_poly3_v3_inline, inputData, inputSize, targetAccuracy);
	*/

	AccuracyTest("sine poly 5", &std::sinf, &sin_poly5, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 5 horner", &std::sinf, &sin_poly5_horner, inputData, inputSize, targetAccuracy);

	/*
	AccuracyTest("sine poly 5 v2", &std::sinf, &sin_poly5_v2, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 5 v3", &std::sinf, &sin_poly5_v3, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 5 v1 inline", &std::sinf, &sin_poly5_v1_inline, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 5 v2 inline", &std::sinf, &sin_poly5_v2_inline, inputData, inputSize, targetAccuracy);
	AccuracyTest("sine poly 5 v3 inline", &std::sinf, &sin_poly5_v3_inline, inputData, inputSize, targetAccuracy);
	*/

	Ceng::AlignedFree(inputData);
}

void LogAccuracyTests()
{
	int groups = 10000000;
	int inputSize = 4 * groups;

	float* inputData = (float*)Ceng::AlignedMalloc(inputSize * sizeof(float), 16);

	for (int k = 0; k < inputSize; k++)
	{
		inputData[k] = abs(rand()) * 0.0001f + 0.000001f;
	}

	printf("steps = %i\n", inputSize);

	float targetAccuracy = 0.005f;

	AccuracyTest("log2_poly3", &std::log2f, &log2_poly3, inputData, inputSize, targetAccuracy);

	Ceng::AlignedFree(inputData);
}

int AccuracyTest(const char* name, SimpleTrigCall reference, SimpleTrigCall callback, float* inputData, int inputSize, float targetAccuracy)
{
	printf("---------------------------------------------------------\n");
	printf("AccuracyTest : %s\n", name);

	int errCount = 0;

	double absErrTotal = 0.0;
	double absErrDevTotal = 0.0;

	for (int k = 0; k < inputSize; k++)
	{
		float testVal = (*callback)(inputData[k]);

		float correct = reference(inputData[k]);

		double absErr = fabs(correct - testVal);

		double absErrDev = fabs(absErr - targetAccuracy);

		absErrTotal += absErr;
		absErrDevTotal += absErrDev;

		if (absErr >targetAccuracy)
		{
			if (errCount < 10)
			{
				printf("ERROR : input = %lf (%lf)\n", inputData[k], inputData[k] * radToDeg);
				printf("\ttestFunc = %lf, expected = %lf\n", testVal, correct);
				printf("\tabsErr = %lf, expected < %lf\n", absErr, targetAccuracy);
				printf("\tabsErrDev = %lf\n", absErrDev);
			}
			errCount++;

			if (errCount == 10)
			{
				printf("Too many errors. Prints suppressed.\n");
			}

		}
	}

	printf("END\n");

	double avgAbsErr = absErrTotal / double(inputSize);
	double avgAbsErrDev = absErrDevTotal / double(inputSize);

	printf("avg abs err = %lf, avg abs err deviation = %lf\n", avgAbsErr, avgAbsErrDev);
	
	if (errCount)
	{
		double errorPercent = double(errCount) / double(inputSize) * 100.0;

		printf("Total errors: %i out of %i (%lf)\n", errCount,inputSize,errorPercent);
	}
	return errCount;
}

void TrigSpeedTests()
{
	printf("******************************************************************\n");
	printf("TrigSpeedTests START\n");

	int groups = 10000000;
	int inputSize = 4 * groups;

	float* inputData = (float*)Ceng::AlignedMalloc(inputSize * sizeof(float), 16);

	for (int k = 0; k < inputSize; k++)
	{
		inputData[k] = rand() * 0.0001f;
	}

	printf("steps = %i\n", inputSize);

	double refDuration = SpeedTest("library sine", &std::sinf, inputData, inputSize, 1.0f);

	SpeedTest("fold_sin_input", &fold_sin_input, inputData, inputSize, refDuration);
	SpeedTest("fold_sin_input_v3", &fold_sin_input_v3, inputData, inputSize, refDuration);
	SpeedTest("fold_sin_input_sse_scalar", &fold_sin_input_sse_scalar, inputData, inputSize, refDuration);

	SpeedTest("sine poly 3 principal", &sin_poly3_principal, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 principal horner", &sin_poly3_principal_horner, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 principal horner sse scalar", &sin_poly3_principal_horner_sse_scalar, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 principal", &sin_poly5_principal, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 principal horner", &sin_poly5_principal_horner, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 principal horner sse scalar", &sin_poly5_principal_horner_sse_scalar, inputData, inputSize, refDuration);

	SpeedTest("sine poly 3", &sin_poly3, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 horner", &sin_poly3_horner, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 sse_scalar", &sin_poly3_sse_scalar, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 sse_scalar inline fold", &sin_poly3_sse_scalar_inline_fold, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 v2", &sin_poly3_v2, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 v3", &sin_poly3_v3, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 v1 inline", &sin_poly3_v1_inline, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 v2 inline", &sin_poly3_v2_inline, inputData, inputSize, refDuration);
	SpeedTest("sine poly 3 v3 inline", &sin_poly3_v3_inline, inputData, inputSize, refDuration);
	
	SpeedTest("sine poly 5", &sin_poly5, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 horner", &sin_poly5_horner, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 sse_scalar", &sin_poly5_sse_scalar, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 sse_scalar inline fold", &sin_poly5_sse_scalar_inline_fold, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 v2", &sin_poly5_v2, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 v3", &sin_poly5_v3, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 v1 inline", &sin_poly5_v1_inline, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 v2 inline", &sin_poly5_v2_inline, inputData, inputSize, refDuration);
	SpeedTest("sine poly 5 v3 inline", &sin_poly5_v3_inline, inputData, inputSize, refDuration);

	Ceng::AlignedFree(inputData);
}

void LogSpeedTests()
{
	printf("******************************************************************\n");
	printf("LogSpeedTests START\n");

	int groups = 10000000;
	int inputSize = 4 * groups;

	float* inputData = (float*)Ceng::AlignedMalloc(inputSize * sizeof(float), 16);

	for (int k = 0; k < inputSize; k++)
	{
		inputData[k] = abs(rand()) * 0.0001f + 0.000001f;
	}

	double refDuration = SpeedTest("library log2", &std::log2f, inputData, inputSize, 1.0f);

	SpeedTest("log2_poly3", &log2_poly3, inputData, inputSize, refDuration);

	Ceng::AlignedFree(inputData);
}


double SpeedTest(const char* name, SimpleTrigCall callback, float* inputData, int inputSize, double refDuration)
{
	printf("---------------------------------------------------------\n");
	printf("SpeedTest : %s\n", name);

	float total = 0.0f;

	double time_start = Ceng_HighPrecisionTimer();

	for (int k = 0; k < inputSize; k++)
	{
		float res = (*callback)(inputData[k]);

		total += res;
	}

	double time_end = Ceng_HighPrecisionTimer();

	double duration = time_end - time_start;

	printf("END\n");
	printf("total = %lf\n", total);
	printf("Duration = %lf , ratio = %lf\n", duration, duration / refDuration);

	return duration;
}