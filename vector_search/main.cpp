
#include <vector>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <ceng/lib/timerlib.h>

#include <ceng/datatypes/aligned-buffer.h>

#include "vector_search.h"

int CheckResult(int* test_result, int* correct_result, int test_size);
void FindAll(int* test_input, int test_size, SearchHandle handle, int* out_result);
int TestSkeleton(const char* name, int* test_input, int test_size, int* correct_result, double refDuration, SearchHandle handle);

void GenerateInput(int test_size, int* out_data)
{
	for (int k = 0; k < test_size; k++)
	{
		out_data[k] = -1;
	}

	for (int k = 0; k < test_size; k++)
	{
		do
		{
			int index = abs(rand()) % test_size;

			if (out_data[index] == -1)
			{
				out_data[index] = k;
				break;
			}
		}
		while (1);
	}
}

void GenerateInput_v2(int test_size, int* out_data)
{
	out_data[0] = abs(rand()) % test_size;

	for (int k = 1; k < test_size; k++)
	{
		do
		{
			int value = abs(rand()) % test_size;

			int found = find_reference(out_data, k, value);

			if (found == -1)
			{
				out_data[k] = value;
				break;
			}

		} while (1);
	}
}

void GenerateInput_v3(int test_size, int* out_data)
{
	for (int k = 0; k < test_size; k++)
	{
		out_data[k] = k;
	}

	for (int k = 0; k < test_size; k++)
	{
		do
		{
			int first = abs(rand()) % test_size;
			int second = abs(rand()) % test_size;

			if (first == second)
			{
				continue;
			}

			std::swap(out_data[first], out_data[second]);
			break;

		} while (1);
	}
}

int main()
{
	double start, end, duration;

	const int num_groups = 10000; // 4;

	const int test_size = 4 * num_groups;

	int* test_input = (int*)Ceng::AlignedMalloc(test_size * sizeof(int), 64);

	printf("Generating test input\n");

	start = Ceng_HighPrecisionTimer();

	GenerateInput_v3(test_size, test_input);

	end = Ceng_HighPrecisionTimer();

	printf("Done\n");

	duration = end - start;

	printf("Took = %lf\n", duration);

	/*
	printf("test input:\n");

	for (int k = 0; k < test_size; k++)
	{
		printf("%i ", test_input[k]);
	}
	printf("\n");
	*/	

	int* correct_result = (int*)std::malloc(test_size * sizeof(int));

	start = Ceng_HighPrecisionTimer();

	FindAll(test_input, test_size, find_reference, correct_result);

	end = Ceng_HighPrecisionTimer();

	double refDuration = end - start;

	printf("RefDuration = %lf , ratio = %lf\n", refDuration, refDuration / refDuration);

	int result;

	result = TestSkeleton("find_group4", test_input, test_size, correct_result, refDuration, find_group4);

	result = TestSkeleton("find_sse", test_input, test_size, correct_result, refDuration, find_sse);

	free(correct_result);

	Ceng::AlignedFree(test_input);

	printf("testing\n");

	return 0;
}

void FindAll(int* test_input, int test_size, SearchHandle handle, int* out_result)
{
	for (int k = 0; k < test_size; k++)
	{
		out_result[k] = handle(test_input, test_size, k);
	}
}

int CheckResult(int* test_result, int* correct_result, int test_size)
{
	int result = 0;

	for (int k = 0; k < test_size; k++)
	{
		if (test_result[k] != correct_result[k])
		{
			printf("ERROR: %i : test_result = %i, correct = %i\n", k, test_result[k], correct_result[k]);
			result++;
		}
		else
		{
			//printf("   OK: %i : test_result = %i, correct = %i\n", k, test_result[k], correct_result[k]);
		}
	}

	if (result > 0)
	{
		printf("Total %i errors\n", result);
	}

	return result;
}

int TestSkeleton(const char* name, int* test_input, int test_size, int* correct_result, double refDuration, SearchHandle handle)
{
	printf("Test : %s : START\n", name);

	int* search_result = (int*)std::malloc(test_size * sizeof(int));

	double start = Ceng_HighPrecisionTimer();

	FindAll(test_input, test_size, handle, search_result);

	double end = Ceng_HighPrecisionTimer();

	double duration = end - start;

	int test_result = CheckResult(search_result, correct_result, test_size);

	std::free(search_result);

	printf("Test : %s : END\n", name);
	printf("Duration = %lf , ratio = %lf\n", duration, duration / refDuration);

	return test_result;
}