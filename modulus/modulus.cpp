
#include <limits>

#include <stdio.h>
#include "popcount.hpp"

uint32_t Div3Reduction(uint32_t x);
bool IsDiv3(uint32_t x);
bool Div3Test();

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

	Div3Test();

	return 0;
}

bool Div3Test()
{
	constexpr uint32_t iters = std::numeric_limits<uint32_t>::max();

	printf("iters = %u\n", iters);

	for (uint32_t k = 0; k < iters; k++)
	{
		if (iters % 10'000 == 0)
		{
			printf("iteration %u\n", k);
		}

		uint32_t rem = k % 3;
		bool correct = (rem == 0);

		bool alg = IsDiv3(k);

		if (correct != alg)
		{
			printf("Error: input = %u, correct = %i, alg = %i\n", k, correct, alg);
			printf("Info: reduction = %u\n", Div3Reduction(k));
			return false;
		}
	}

	return true;
}

uint32_t Div3Reduction(uint32_t x)
{
	constexpr uint32_t EVEN_BIT_MASK = 0b01010101010101010101010101010101;
	constexpr uint32_t ODD_BIT_MASK = EVEN_BIT_MASK ^ -1;

	x = PopCount(x & EVEN_BIT_MASK) + 2 * PopCount(x & ODD_BIT_MASK);

	// After first round x fits within 6 bits

	x = PopCount(x & EVEN_BIT_MASK) + 2 * PopCount(x & ODD_BIT_MASK);

	return x;
}


bool IsDiv3(uint32_t x)
{
	uint32_t reduced = Div3Reduction(x);

	return (reduced == 6) || (reduced == 3) || (reduced == 0);
}