#include "vector_search.h"

int find_reference(int* array, int length, int x)
{
	for (int k = 0; k < length; k++)
	{
		if (array[k] == x)
		{
			return k;
		}
	}

	return -1;
}

int find_sse4(int* array, int length, int x)
{
	return -1;
}