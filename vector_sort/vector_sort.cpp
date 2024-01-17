/*****************************************************************************
*
* alg-test.cpp
*
* By Jari Korkala 5/3/2016
*
* For purposes of testing SIMD algorithms.
*
*****************************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>

#include <ceng/lib/timerlib.h>

#include "sse-sort.h"

#include <ceng/datatypes/aligned-buffer.h>


void round_trip_test(const unsigned char *input);

void accuracy_test(float *exact, float *test, const int size);

int sort_test();

int uint8_to_normalized_float_tests();

int main()
{
	sort_test();

	//uint8_to_normalized_float_tests();

	return 0;
}

void accuracy_test(float *exact, float *test, const int size)
{
	for (int k = 0; k < size; ++k)
	{
		std::cout << "comparison at " << k << " : " << exact[k] << " , " << test[k]
			<< " | delta = " << std::fabs(exact[k] - test[k]) << std::endl;
	}
}

void round_trip_test(const unsigned char *input)
{
	std::cout << "round trip results:" << std::endl;

	for (int k = 0; k < 256; ++k)
	{
		if (input[k] != k)
		{
			std::cout << "k = " << k << " , round = " << int(input[k]) << std::endl;
		}
	}
}

typedef void (*sort4_callback)(float* input4, const int size);

int sort4_test(const char* name, sort4_callback callback, float* input, float* correct_output, const int size, double base_duration)
{
	double start, end, duration;

	float* temp_input = (float*)Ceng::AlignedMalloc(size * sizeof(float), 64);

	memcpy(temp_input, input, size * sizeof(float));

	start = Ceng_HighPrecisionTimer();

	callback(temp_input, size);

	end = Ceng_HighPrecisionTimer();

	duration = end - start;

	std::cout << name << ": " << duration << ", ratio = " << duration / base_duration << std::endl;

	std::cout << "correctness check:" << std::endl;

	int errorCount = 0;

	int groups = size / 4;

	for (int i = 0; i < groups; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			if (temp_input[4 * i + k] != correct_output[4 * i + k])
			{
				std::cout << "Mismatch: " << k << ": found = " << temp_input[4 * i + k] << ", expected = " << correct_output[4 * i + k] << std::endl;
				errorCount++;
			}
		}
	}

	if (errorCount == 0)
	{
		std::cout << "No errors" << std::endl;
	}

	Ceng::AlignedFree(temp_input);

	return 0;
}

int sort_test()
{
	const int test_size = 4 * 10000000;

	int groups = test_size / 4;

	float* input = (float*)Ceng::AlignedMalloc(test_size * sizeof(float), 64);
	float* correct_output = (float*)Ceng::AlignedMalloc(test_size * sizeof(float), 64);


	std::cout << "*******************************************************" << std::endl;
	std::cout << "sort test" << std::endl;

	std::cout << "randomizing input" << std::endl;
	for (int k = 0; k < test_size; ++k)
	{
		input[k] = (rand() % 257) / 256.0f;
	}

	input[0] = 4.0;
	input[1] = 3.0;
	input[2] = 2.0;
	input[3] = 1.0;

	/*
	std::cout << "Input dump:" << std::endl;

	for (int i = 0; i < groups; i++)
	{
		std::cout << "-------------------------------------" << std::endl;
		for (int k = 0; k < 4; k++)
		{
			std::cout << "input[" << k << "] = " << input[4*i+k] << std::endl;
		}
	}

	std::cout << "-------------------------------------" << std::endl;
	*/
	

	double start, end;
	double base_duration;

	memcpy(correct_output, input, test_size * sizeof(float));

	start = Ceng_HighPrecisionTimer();

	float_sort4_stdlib_qsort(correct_output, test_size);

	end = Ceng_HighPrecisionTimer();

	base_duration = end - start;

	/*
	std::cout << "correct output dump:" << std::endl;

	for (int i = 0; i < groups; i++)
	{
		std::cout << "-------------------------------------" << std::endl;
		for (int k = 0; k < 4; k++)
		{
			std::cout << "out[" << k << "] = " << correct_output[4 * i + k] << std::endl;
		}
	}

	std::cout << "-------------------------------------" << std::endl;
	*/

	std::cout << "stdlib quicksort: " << base_duration << std::endl;

	
	sort4_test("selection sort", float_sort4_selection_sort, input, correct_output, test_size, base_duration);
	sort4_test("insertion sort", float_sort4_insertion_sort, input, correct_output, test_size, base_duration);
	sort4_test("minmax sort STL", float_sort4_minmax_stl, input, correct_output, test_size, base_duration);
	sort4_test("minmax sort STL int punning", float_sort4_minmax_stl_int_punning, input, correct_output, test_size, base_duration);
	sort4_test("minmax sort x87 cmov", float_sort4_minmax_x87_cmov, input, correct_output, test_size, base_duration);
	sort4_test("minmax sort x87 cmov v2", float_sort4_minmax_x87_cmov_v2, input, correct_output, test_size, base_duration);
	sort4_test("minmax SSE scalar", float_sort4_minmax_sse_scalar, input, correct_output, test_size, base_duration);
	sort4_test("minmax SSE vector", float_sort4_minmax_sse, input, correct_output, test_size, base_duration);

	sort4_test("minmax SSE vector v2", float_sort4_minmax_sse_v2, input, correct_output, test_size, base_duration);



	Ceng::AlignedFree(input);
	Ceng::AlignedFree(correct_output);

	return 0;
}


