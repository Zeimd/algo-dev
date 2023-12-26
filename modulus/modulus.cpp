
#include <limits>

#include <stdio.h>
#include "popcount.hpp"

uint32_t Div3Reduction(uint32_t x);
bool IsDiv3(uint32_t x);
uint32_t Mod3(uint32_t x);
bool Div3Test();

typedef uint32_t(*TestFunction)(uint32_t);

bool PopCountTest(TestFunction func);

int main()
{
	uint32_t input = 277;

	uint32_t pop = PopCount(input);

	printf("in = %u, popcount = %u\n", input, pop);

	uint32_t remainder = input % 3;

	printf("remainder = %u\n", remainder);

	bool div3 = remainder == 0;

	printf("is div by 3 = %i\n", div3);

	div3 = IsDiv3(input);

	//PopCountTest(PopCountSWAR);
	PopCountTest(PopCountSWARv2);

	//Div3Test();

	return 0;
}

bool PopCountTest(TestFunction func)
{
	bool result = true;

	printf("PopCountTest: start\n");

	constexpr uint32_t iters = std::numeric_limits<uint32_t>::max();

	printf("iters = %u\n", iters);

	for (uint32_t k = 0; k < iters; k++)
	{
		if (k % 10'000 == 0)
		{
			printf("iteration %u\n", k);
		}

		uint32_t correct = PopCount(k);

		uint32_t test = (*func)(k);

		if (correct != test)
		{
			printf("Error: input = %u, correct = %u, swar = %u\n", k, correct, test);
			result = false;
		}
	}

	printf("PopCountTest: end\n");

	return result;
}

bool Div3Test()
{
	bool result = true;

	constexpr uint32_t iters = std::numeric_limits<uint32_t>::max();

	printf("Div3Test: start\n");

	printf("iters = %u\n", iters);

	for (uint32_t k = 0; k < iters; k++)
	{
		if ( k % 10'000 == 0)
		{
			printf("iteration %u\n", k);
		}

		uint32_t rem = k % 3;
		bool correct = (rem == 0);

		uint32_t alg_rem = Mod3(k);

		bool isdiv = IsDiv3(k);

		if ( (rem != alg_rem) || (correct != isdiv) )
		{
			printf("Error: input = %u\n", k);
			printf("correct rem = %u, alg rem = %u\n", rem, alg_rem);
			printf("correct isdiv = %i, alg isdiv = %i\n", correct, isdiv);

			uint32_t reduction = k;

			for (int j = 0; j < 4; j++)
			{
				reduction = Div3Reduction(reduction);
				printf("Info: reduction %i = %u\n", j,reduction);
			}
			result = false;
			break;
		}
	}

	printf("Div3Test: end\n");

	return result;
}

uint32_t Div3Reduction(uint32_t x)
{
	constexpr uint32_t EVEN_BIT_MASK = 0b01010101010101010101010101010101;
	constexpr uint32_t ODD_BIT_MASK = EVEN_BIT_MASK ^ -1;

	x = PopCountSWAR(x & EVEN_BIT_MASK) + 2 * PopCountSWAR(x & ODD_BIT_MASK);

	return x;
}

uint32_t Mod3(uint32_t x)
{
	uint32_t reduction = Div3Reduction(x);

	// First iteration produces a result in [0,48] range (6 bits)

	reduction = Div3Reduction(reduction);

	// Second iteration produces a result in [0,9] range (4 bits)

	reduction = Div3Reduction(reduction);

	// Third iteration produces a result in [0,6] range (3 bits)

	reduction = Div3Reduction(reduction);

	// Fourth iteration produces a result in [0,3] range (2 bits)

	if (reduction == 3)
	{
		return 0;
	}
	return reduction;
}

bool IsDiv3(uint32_t x)
{
	uint32_t reduction = Div3Reduction(x);

	// First iteration produces a result in [0,48] range (6 bits)

	reduction = Div3Reduction(reduction);

	return (reduction == 6) || (reduction == 3) || (reduction == 0);
}