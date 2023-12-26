
#include "popcount.hpp"

uint32_t PopCount(uint32_t x)
{
	uint32_t counter = 0;

	for (int k = 0; k < 32; k++)
	{
		counter += (x & 1);
		x >>= 1;
	}

	return counter;
}