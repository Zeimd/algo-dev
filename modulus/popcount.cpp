
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

uint32_t PopCountSWAR(uint32_t x)
{
	constexpr uint32_t SUM_MASK = (1 | (1 << 8) | (1 << 16) | (1 << 24));

	uint32_t counter = 0;

	for (int k = 0; k < 8; k++)
	{
		counter += (x & SUM_MASK);
		x >>= 1;
	}

	counter = (counter & 65535) + ((counter >> 16));
	counter = (counter & 255) + ((counter >> 8));

	return counter;
}