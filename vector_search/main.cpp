
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

int main()
{
	const int num_groups = 4;

	const int test_size = 4 * num_groups;

	int* test_input = (int*)Ceng::AlignedMalloc(test_size * sizeof(int), 64);

	for (int k = 0; k < test_size; k++)
	{
		test_input[k] = -1;
	}

	for (int k = 0; k < test_size; k++)
	{
		do
		{
			int index = abs(rand()) % test_size;

			if (test_input[index] == -1)
			{
				test_input[index] = k;
				break;
			}
		}
		while (1);
	}

	printf("test input:\n");

	for (int k = 0; k < test_size; k++)
	{
		printf("%i ", test_input[k]);
	}
	printf("\n");

	int* correct_result = (int*)std::malloc(test_size * sizeof(int));

	double start = Ceng_HighPrecisionTimer();

	FindAll(test_input, test_size, find_reference, correct_result);

	double end = Ceng_HighPrecisionTimer();

	double refDuration = end - start;

	printf("RefDuration = %lf , ratio = %lf\n", refDuration, refDuration / refDuration);

	int result = TestSkeleton("find_sse", test_input, test_size, correct_result, refDuration, find_sse);

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
			printf("   OK: %i : test_result = %i, correct = %i\n", k, test_result[k], correct_result[k]);
		}
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