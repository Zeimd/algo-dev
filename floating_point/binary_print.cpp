#include <stdio.h>

#include "binary_print.h"

void PrintBinary(const uint8_t x)
{
	for (int k = 0; k < 8; k++)
	{
		printf("%u", ((x >> (7 - k)) & 1));
	}
	printf(" ");
}

void PrintBinary(const uint8_t* x, const int32_t bytes)
{
	for (int32_t k = bytes - 1; k >= 0; k--)
	{
		PrintBinary(x[k]);
	}
}

void PrintBinary(const uint16_t x)
{
	PrintBinary((uint8_t*)&x, 2);
}

void PrintBinary(const uint32_t x)
{
	PrintBinary((uint8_t*)&x, 4);
}

void PrintBinary(const uint64_t x)
{
	PrintBinary((uint8_t*)&x, 8);
}

void PrintBinary(const int16_t x)
{
	PrintBinary((uint8_t*)&x, 2);
}

void PrintBinary(const int32_t x)
{
	PrintBinary((uint8_t*)&x, 4);
}

void PrintBinary(const int64_t x)
{
	PrintBinary((uint8_t*)&x, 8);
}

void PrintBinary(const float x)
{
	uint32_t* punning = (uint32_t*)&x;

	printf("%u ", (*punning >> 31));
	
	PrintBinary(uint8_t((*punning >> 23) & 255));

	printf(" ");

	for (int k = 0; k < 23; k++)
	{
		printf("%u", ((*punning >> (22 - k)) & 1));
	}
	printf(" ");
}

void PrintBinary(const Float16 x)
{
	uint32_t value = x.raw;

	printf("%u ", (value >> 15));

	for (int k = 0; k < 5; k++)
	{
		printf("%u", ((value >> (14 - k)) & 1));
	}

	printf(" ");

	for (int k = 0; k < 10; k++)
	{
		printf("%u", ((value >> (9 - k)) & 1));
	}
}