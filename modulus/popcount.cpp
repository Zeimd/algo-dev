
#include "popcount.hpp"

// Reference implementation for population count (set bits).
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

// Optimized population count that employs SIMD within a register strategy.
uint32_t PopCountSWAR(uint32_t x)
{
	constexpr uint32_t SUM_MASK = (1 | (1 << 8) | (1 << 16) | (1 << 24));

	// We treat the counter as 4x byte

	// counter = [ s3,s2,s1,s0 ]

	uint32_t counter = 0;

	for (int k = 0; k < 8; k++)
	{
		counter += (x & SUM_MASK);
		x >>= 1;
	}

	counter = counter + (counter >> 16);

	// counter = [0,0,a1,a0]

	counter = (counter + (counter >> 8)) & 255;

	// counter = [0,0,0,popcount]

	return counter;
}

// Further optimized population count that employs SIMD within a register strategy.
uint32_t PopCountSWARv2(uint32_t x)
{
	constexpr uint32_t SUM_MASK = (1 | (1 << 4) | (1 << 8) | (1 << 12) | (1 << 16) | (1 << 20) | (1 << 24) | (1 << 28));

	constexpr uint32_t COUNTER_MASK_EVEN = (15 | (15 << 8) | (15 << 16) | (15 << 24));

	// We treat the counter as 8x half-byte (4 bits)

	// counter = [ s7,s6,s5,s4,s3,s2,s1,s0 ]

	uint32_t counter = 0;

	for (int k = 0; k < 4; k++)
	{
		counter += (x & SUM_MASK);
		x >>= 1;
	}

	// Now each half-byte sum has format [0,nnn] bits, since 4 is highest value possible.
	// Because of this it is possible to add two half-byte counters and result still fits in 4 bits

	counter = (counter + (counter >> 4)) & COUNTER_MASK_EVEN;

	// Now the half-bytes are [ 0,a3,0,a2,0,a1,0,a0 ]
	// Where [0,aX] form valid unsigned byte counters. From this point onward algorithm works like SWARv1.

	counter = counter + (counter >> 16);

	// counter = [0,0,b1,b0]

	counter = (counter + (counter >> 8)) & 255;

	// counter = [0,0,0,popcount]

	return counter;
}