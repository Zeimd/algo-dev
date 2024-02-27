#include <stdio.h>
#include <math.h>

#include <algorithm>

#include "solid_angle.h"


int SolidAngleTests();
int SolidAngleAccuracyTest(const char* name, int cubeWidth, SolidAngleTable& reference, SolidAngleTable& testTable);

int main()
{
	SolidAngleTests();

	return 0;
}

int SolidAngleTests()
{
	printf(__func__);
	printf("\n");

	const int cubeWidth = 256;

	SolidAngleTable reference{ cubeWidth };
	SolidAngleTableQuadrant quadrant{ cubeWidth };

	/*
	float correct, test;

	correct = reference.Get(126, 0);
	test = quadrant.Get(126, 0);

	printf("(126,0) : test = %.15lf, correct = %.15lf\n", test, correct);

	correct = reference.Get(127, 0);
	test = quadrant.Get(127, 0);

	printf("(126,0) : test = %.15lf, correct = %.15lf\n", test, correct);

	correct = reference.Get(128, 0);
	test = quadrant.Get(128, 0);

	printf("(128,0) : test = %.15lf, correct = %.15lf\n", test, correct);

	correct = reference.Get(129, 0);
	test = quadrant.Get(129, 0);

	printf("(129,0) : test = %.15lf, correct = %.15lf\n", test, correct);
	*/
	

	SolidAngleAccuracyTest("quadrant table",cubeWidth, reference, quadrant);

	return 0;
}

int SolidAngleAccuracyTest(const char* name, int cubeWidth, SolidAngleTable& reference, SolidAngleTable& testTable)
{
	printf(name);
	printf("\n");

	int errorCount = 0;

	float maxAbsErr = 0.0f;
	float maxRelErr = 0.0f;

	int maxAbsErrU = 0, maxAbsErrV = 0;
	int maxRelErrU = 0, maxRelErrV = 0;

	for (uint32_t v = 0; v < cubeWidth; v++)
	{
		for (uint32_t u = 0; u < cubeWidth; u++)
		{
			float correct = reference.Get(u, v);
			float test = testTable.Get(u, v);

			if (test != correct)
			{
				float absErr = fabsf(test - correct);
				float relErr = absErr / fabs(correct);

				maxAbsErr = std::max(absErr, maxAbsErr);

				if (maxAbsErr == absErr)
				{
					maxAbsErrU = u;
					maxAbsErrV = v;
				}

				maxRelErr = std::max(relErr, maxRelErr);

				if (maxRelErr == relErr)
				{
					maxRelErrU = u;
					maxRelErrV = v;
				}

				if (errorCount < 100)
				{
					printf("ERROR: (%i,%i) : test = %.15lf, correct = %.15lf\n", u, v, test, correct);
					printf("absErr=%.15lf, relErr=%.15lf\n", absErr, relErr);
				}

				errorCount++;

				if (errorCount == 100)
				{
					printf("Too many errors. Printing suppressed\n");
				}
			}
		}
	}

	int totalCases = cubeWidth * cubeWidth;

	double percent = double(errorCount) / double(totalCases) * 100.0;

	printf("Total errors = %i / %i (%lf)\n", errorCount, totalCases, percent);
	printf("maxAbsErr = %.15lf (%i,%i), maxRelErr = %.15lf (%i,%i)\n", maxAbsErr, maxAbsErrU, maxAbsErrV,
		maxRelErr, maxRelErrU, maxRelErrV);

	return 0;
}