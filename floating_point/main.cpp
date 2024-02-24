#include <vector>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <limits>

#include <ceng/lib/timerlib.h>

#include <ceng/datatypes/aligned-buffer.h>

#include "float16.h"

#include "binary_print.h"

int FloatToHalfRoundtripTest();

int main()
{
	/*
	printf("halfNormalMin = %.15f | ", halfNormalMin);
	PrintBinary(halfNormalMin);
	printf("\n");

	printf("halfSubnormalMin = %.15f | ", halfSubnormalMin);
	PrintBinary(halfSubnormalMin);
	printf("\n");
	
	float x = 0.000000059605320f;

	printf("input = %.15f | ", x);
	PrintBinary(x);
	printf("\n");

	Float16 y(x);

	PrintBinary(y);
	printf("\n");

	float z = y;

	printf("output = %.15f | ", z);
	PrintBinary(z);
	printf("\n");
	*/


	FloatToHalfRoundtripTest();

	return 0;
}

int FloatToHalfRoundtripTest()
{
	printf(__func__);

	float value;

	uint32_t* punning = (uint32_t*)&value;

	uint32_t errorCount = 0;

	for (uint32_t k = 0; k < std::numeric_limits<uint32_t>::max(); k++)
	{
		*punning = k;

		if (value < halfSubnormalMin || value > halfMax)
		{
			continue;
		}

		Float16 y(value);
		float z = y;

		if (z != value)
		{
			if (errorCount < 100)
			{
				float absErr = fabsf(value - z);

				printf("ERROR: input = %.15f, output = %.15f \n", value, z);
				printf("absolute error = %.15f\n", absErr);
				printf("Input : ");
				PrintBinary(value);
				printf("\n");
				printf("Float 16 : ");
				PrintBinary(y);
				printf("\n");
				printf("output : ");
				PrintBinary(z);
				printf("\n");				
			}

			errorCount++;

			if (errorCount == 100)
			{
				printf("Too many errors. Printing supressed\n");
			}
		}
	}

	double errorPercent = double(errorCount) / double(std::numeric_limits<uint32_t>::max()) * 100.0;

	printf("Total errors = %u / %u (%f %%)\n", errorCount, std::numeric_limits<uint32_t>::max(), errorPercent);

	return errorCount;
}
