// division.cpp : Defines the entry point for the application.
//

#include <vector>

#include <intrin.h>
#include <stdint.h>
#include <stdio.h>

#include <ceng/lib/timerlib.h>

uint32_t RestoringDivision(const uint32_t a, const uint32_t b, uint32_t* remainder);

uint32_t RestoringDivisionRadix4(const uint32_t a, const uint32_t b, uint32_t* remainder);

uint32_t RestoringDivisionRadix16(const uint32_t a, const uint32_t b, uint32_t* remainder);

typedef uint32_t (*DivisionCall)(const uint32_t a, const uint32_t b, uint32_t* remainder);

void InitRadix16Table();

void PrintBinary(const uint8_t x)
{
	for (int k = 0; k < 8; k++)
	{
		printf("%u", ((x >> (7 - k)) & 1));
	}
	printf(" ");
}

void PrintBinary(const uint8_t *x, const int32_t bytes)
{
	for (int32_t k = bytes - 1; k >= 0; k--)
	{
		PrintBinary(x[k]);
	}
}

void PrintBinary(const uint32_t x)
{
	PrintBinary((uint8_t*)&x, 4);
}

void PrintBinary(const uint64_t x)
{
	PrintBinary((uint8_t*)&x, 8);
}

void PrintBinary(const int32_t x)
{
	PrintBinary((uint8_t*)&x, 4);
}

void PrintBinary(const int64_t x)
{
	PrintBinary((uint8_t*)&x, 8);
}

struct DivisionInput
{
	uint32_t numerator;
	uint32_t divisor;
};

struct DivisionResult
{
	uint32_t result;
	uint32_t remainder;
};

int DivisionTest(const char* name, DivisionCall callback, const std::vector<DivisionInput>& input);
double SpeedTest(const char* name, DivisionCall callback, const std::vector<DivisionInput>& input, double refDuration);

int DivisionTests(const std::vector<DivisionInput>& input);
void SpeedTests(const std::vector<DivisionInput>& input);

int main()
{
	InitRadix16Table();

	uint32_t a = 32;
	uint32_t b = 11;

	//uint32_t a = 123456;
	//uint32_t b = 113;

	//uint32_t a = 333;
	//uint32_t b = 1;

	printf("a = %u\n", a);
	PrintBinary(a);
	printf("\n");

	printf("b = %u\n", b);
	PrintBinary(b);
	printf("\n");

	uint32_t result, remainder;

	printf("Correct:\n");

	result = a / b;

	printf("result = %u\n", result);
	PrintBinary(result);
	printf("\n");

	remainder = a % b;

	printf("remainder = %u\n", remainder);
	PrintBinary(remainder);
	printf("\n");

	/*
	printf("Restoring division:\n");

	result = RestoringDivision(a, b, &remainder);

	printf("result = %u\n", result);
	PrintBinary(result);
	printf("\n");

	printf("remainder = %u\n", remainder);
	PrintBinary(remainder);
	printf("\n");
	*/

	printf("Restoring division radix 4:\n");

	result = RestoringDivisionRadix4(a, b, &remainder);

	printf("result = %u\n", result);
	PrintBinary(result);
	printf("\n");

	printf("remainder = %u\n", remainder);
	PrintBinary(remainder);
	printf("\n");

	/*
	printf("Restoring division radix 16:\n");

	result = RestoringDivisionRadix16(a, b, &remainder);

	printf("result = %u\n", result);
	PrintBinary(result);
	printf("\n");

	printf("remainder = %u\n", remainder);
	PrintBinary(remainder);
	printf("\n");
	*/

	uint32_t range = 1 << 25;// 20000;

	std::vector<DivisionInput> input(range);

	printf("input size = %i\n", range);
	printf("input vector size = %i\n", input.size());

	for (uint32_t k = 0; k < range; k++)
	{
		do
		{
			input[k].numerator = abs(rand());
		} while (input[k].numerator == 0);

		do
		{
			input[k].divisor = abs(rand());
		} while (input[k].divisor == 0);
	}

	SpeedTests(input);
	//DivisionTests(input);

	return 0;
}

int DivisionTests(const std::vector<DivisionInput>& input)
{
	DivisionTest("RestoringDivision", RestoringDivision, input);
	DivisionTest("RestoringDivisionRadix4", RestoringDivisionRadix4, input);
	DivisionTest("RestoringDivisionRadix16", RestoringDivisionRadix16, input);

	return 0;
}


int DivisionTest(const char* name, DivisionCall callback, const std::vector<DivisionInput>& input)
{
	printf("DivisionTest\n");

	printf("TEST: %s START\n", name);

	int errorCount = 0;

	for (auto&x : input)
	{
		uint32_t correctDiv, correctMod;

		correctDiv = x.numerator / x.divisor;
		correctMod = x.numerator % x.divisor;

		uint32_t testDiv, testMod;

		testDiv = callback(x.numerator, x.divisor, &testMod);

		if (correctDiv != testDiv || correctMod != testMod)
		{
			printf("ERROR: n = %u, d = %u\n", x.numerator, x.divisor);
			printf("testDiv = %u, correct = %u\n", testDiv, correctDiv);
			printf("testMod = %u, correct = %u\n", testMod, correctMod);
		}
	}

	printf("TEST: %s END\n", name);

	if (errorCount > 0)
	{
		printf("Total %i errors\n", errorCount);
	}

	return errorCount;
}

uint32_t NativeDivision(const uint32_t a, const uint32_t b, uint32_t* remainder)
{
	*remainder = a % b;
	return a / b;
}

void SpeedTests(const std::vector<DivisionInput>& input)
{
	printf("SpeedTests\n");

	double refDuration = SpeedTest("Native division", NativeDivision, input, 1.0);

	SpeedTest("RestoringDivision", RestoringDivision, input, refDuration);
	SpeedTest("RestoringDivisionRadix4", RestoringDivisionRadix4, input, refDuration);
	SpeedTest("RestoringDivisionRadix16", RestoringDivisionRadix16, input, refDuration);
}

double SpeedTest(const char* name, DivisionCall callback, const std::vector<DivisionInput>& input, double refDuration)
{
	printf("SpeedTest: %s START\n", name);

	double start = Ceng_HighPrecisionTimer();

	uint64_t accumulator = 0;

	for (auto& x : input)
	{
		uint32_t testDiv, testMod;

		testDiv = callback(x.numerator, x.divisor, &testMod);

		accumulator += uint64_t(testDiv) + testMod;
	}

	double end = Ceng_HighPrecisionTimer();

	printf("TEST: %s END\n", name);

	double duration = end - start;

	printf("Duration = %lf , ratio = %lf\n", duration, duration / refDuration);
	printf("accumulator = %llu\n", accumulator);

	return duration;
}

uint32_t RestoringDivision(const uint32_t a, const uint32_t b, uint32_t *remainder)
{
	if (b > a)
	{
		*remainder = a;
		return 0;
	}

	// Scale divisor so that most significat bit of numerator
	// and divisor match

	uint32_t nBits = 32-__lzcnt(a);
	
	int64_t n = int64_t(a);

	int64_t divisor = int64_t(b) << nBits;

#ifdef _DEBUG
	printf("[DEBUG] nBits = %i\n", nBits);

	printf("[DEBUG] numerator = %llu\n", n);
	PrintBinary(n);
	printf("\n");

	printf("[DEBUG] divisor = %llu\n", divisor);
	PrintBinary(divisor);
	printf("\n");
#endif

	uint32_t result = 0;
	uint32_t bit = 0;

	for(uint32_t k=0; k< nBits; k++)
	{
		n = n << 1;
		n -= divisor;

		bit = 1;
		if (n < 0)
		{
			n += divisor;
			bit = 0;
		}

		result = (result << 1) | bit;
	}

#ifdef _DEBUG
	printf("[DEBUG] result = %u\n", result);
	PrintBinary(result);
	printf("\n");
	printf("[DEBUG] remainder = %llu\n", n);
	PrintBinary(n);
	printf("\n");
#endif

	*remainder = (uint32_t)n >> nBits;

	return result;
}

// Lookup table for radix 4 division. The equation for table
// values is
//
// table[d][nnnn] = nnnn / (2+d)
//
// Explanation:
//
// At initialization the divisor is shifted so that its bit 0 is 
// aligned with the most significant set bit of the numerator. 
//
// Next, the position of the most significant set bit of the
// divisor is stored.
//
// if b = msb(divisor), then
//
// d = read_bit(divisor, b-1)
//
// which is a constant for the entire division
//
// Now, nnnn is always read from numerator bits [b-1 : b+2]
//
// So basically if we look at the numerator and divisor
// contents, we always have the following situation:
//
//           Original MSB of numerator
//           v
//	 nnnn xxxxxxxxxxxxxxxxxxx
//	 001d yyyy000000000000000
//     ^
//     MSB of divisor
//
// where we don't care of the xxxx and yyyy bits when
// selecting the table value.
//
// In practice the value that cames out is in range [0,3]
// since a smaller multiplier gets detected before shifting
// causes the largest nnnn values to appear.
//
// the number of inner table entries used is given by
//
// 3*(2+d)
//
static const int32_t lookupTableRadix4[2][16] =
{
	{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7},
	{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5}
};

// Restoring division that produces 2 bits of result
// per iteration.
// NOTE: result undefined if b = 0.
uint32_t RestoringDivisionRadix4(const uint32_t a, const uint32_t b, uint32_t* remainder)
{
	if (b > a)
	{
		*remainder = a;
		return 0;
	}

	uint32_t nBits = 32 - __lzcnt(a);
	uint32_t dBits = 32 - __lzcnt(b);

#ifdef _DEBUG
	printf("[DEBUG] nBits = %u, dBits=%u\n", nBits, dBits);
#endif

	// Move numerator to 64 bits so that we can shift left even
	// if numerator uses all 32 bits

	int64_t n = int64_t(a);

	// ceil nBits to nearest multiple of two. This ensures that
	// we don't introduce a trailing zero if nBits is odd
	nBits = (nBits + 1) & ~1;

	// Scale divisor so that bit 0 of divisor is aligned with
	// most significant set bit of the numerator

	int64_t divisor = int64_t(b) << nBits;

	// The numerator and divisor are always aligned like
	// this:
	//
	//           Original MSB of numerator
	//           v
	//	 nnnn xxxxxxxxxxxxxxxxxxx
	//	 001d yyyy000000000000000
	//     ^
	//     MSB of divisor

	// Calculate the right shift needed to get nnnn field
	// to lowest 4 bits. 
	uint32_t nKeyShift = nBits + dBits - 2;

	// Extract d bit from the above layout. Basically
	// it decides whether we use lookup table for division
	// by 2 or 3.

	uint32_t dKey = (divisor >> (nKeyShift)) & 1;

#ifdef _DEBUG
	printf("[DEBUG] nBits = %i, nKeyShift=%u, dKey=%u\n", nBits, nKeyShift, dKey);

	printf("[DEBUG] numerator = %llu\n", n);
	PrintBinary(n);
	printf("\n");

	printf("[DEBUG] divisor = %llu\n", divisor);
	PrintBinary(divisor);
	printf("\n");
#endif

	uint32_t result = 0;


	for (uint32_t k = 0; k < nBits; k+=2)
	{
		n = n << 2;

#ifdef _DEBUG
		printf("[DEBUG] iteration = %u\n", k);

		printf("[DEBUG] n = %lli\n", n);
		PrintBinary(n);
		printf("\n");
		PrintBinary(divisor);
		printf("\n");
#endif

		int32_t nKey = (n >> nKeyShift) & 15;

		int32_t bit = lookupTableRadix4[dKey][nKey];

		int64_t sub = int64_t(bit)*divisor;

#ifdef _DEBUG
		printf("[DEBUG] nKey = ");
		PrintBinary(nKey);
		printf("\n");
		printf("[DEBUG] dKey = ");
		PrintBinary(dKey);
		printf("\n");

		printf("[DEBUG] bit = %u\n", bit);
		printf("[DEBUG] sub = %lli\n", sub);
		PrintBinary(sub);
		printf("\n");
#endif

		n -= sub;

#ifdef _DEBUG
		printf("[DEBUG] n after sub = %lli\n", n);
		PrintBinary(n);
		printf("\n");
#endif

		// Since the value from the lookup table uses
		// only the highest bits of numerator and divisor
		// to decide the multiplier, we might end up
		// subtracting too much. In this case it's necessary
		// to decrease multiplier by one to keep numerator
		// positive
		while (n < 0)
		{
#ifdef _DEBUG
			printf("[DEBUG] adjustment needed\n");
#endif
			n += divisor;
			bit--;

#ifdef _DEBUG
			printf("[DEBUG] n = %lli\n", n);
			PrintBinary(n);
			printf("\n");

			printf("[DEBUG] bit = %u\n", bit);
#endif
		}

		// Store 2 bits of result
		result = (result << 2) | bit;

#ifdef _DEBUG
		printf("[DEBUG] intermediate result = ");
		PrintBinary(result);
		printf("\n");
#endif
	}

#ifdef _DEBUG
	printf("[DEBUG] result = %u\n", result);
	PrintBinary(result);
	printf("\n");
	printf("[DEBUG] remainder = %llu\n", n);
	PrintBinary(n);
	printf("\n");
#endif

	* remainder = (uint32_t)n >> nBits;

	return result;
}

// This lookup table is for radix 16 restoring division
//
// Definition:
//
// table[d][n] = n / (8+d)
//
// It is huge, but only a subset of each inner table is used.
// This is because 15 is the largest multiplier we can get when we produce
// 4 bits of result per iteration. This means that the number of
// inner table entries used is
//
// 15*(8+d)
//
// This means that we can crop the outer table to 225 entries.
// TODO: why are all 256 table entries still needed?
static int32_t lookupTableRadix16[8][256];

void InitRadix16Table()
{
	for (int d = 0; d < 8; d++)
	{
		for (int n = 0; n < 256; n++)
		{
			lookupTableRadix16[d][n] = n / (8 + d);
		}
	}
}


// Restoring division that produces 4 bits of result
// per iteration.
// NOTE: result undefined if b = 0.
uint32_t RestoringDivisionRadix16(const uint32_t a, const uint32_t b, uint32_t* remainder)
{
	if (b > a)
	{
		*remainder = a;
		return 0;
	}

	uint32_t nBits = 32 - __lzcnt(a);
	uint32_t dBits = 32 - __lzcnt(b);

#ifdef _DEBUG
	printf("[DEBUG] nBits = %u, dBits=%u\n", nBits, dBits);
#endif

	// Move numerator to 64 bits so that we can shift left even
	// if numerator uses all 32 bits

	int64_t n = int64_t(a);

	// ceil nBits to nearest multiple of four. This ensures that
	// we don't introduce trailing zeroes if nBits != 4*k
	nBits = (nBits + 3) & ~3;

	// Scale divisor so that bit 0 of divisor is aligned with
	// most significant set bit of the numerator

	int64_t divisor = int64_t(b) << nBits;

	// The numerator and divisor are always aligned like
	// this:
	//
	//               Original MSB of numerator
	//               v
	//	 nnnnnnnn xxxxxxxxxxxxxxxxxxx
	//	 00001ddd yyyy000000000000000
	//       ^
	//       MSB of divisor

	// Calculate the right shift needed to get nnnnnnnn field
	// to lowest 8 bits. 
	uint32_t nKeyShift = nBits + dBits - 4;

	// Extract d key from the above layout.

	uint32_t dKey = (divisor >> (nKeyShift)) & 7;

#ifdef _DEBUG
	printf("[DEBUG] nBits = %i, nKeyShift=%u, dKey=%u\n", nBits, nKeyShift, dKey);

	printf("[DEBUG] numerator = %llu\n", n);
	PrintBinary(n);
	printf("\n");

	printf("[DEBUG] divisor = %llu\n", divisor);
	PrintBinary(divisor);
	printf("\n");
#endif

	uint32_t result = 0;


	for (uint32_t k = 0; k < nBits; k += 4)
	{
		n = n << 4;

#ifdef _DEBUG
		printf("[DEBUG] iteration = %u\n", k >> 2);

		printf("[DEBUG] n = %lli\n", n);
		PrintBinary(n);
		printf("\n");
		PrintBinary(divisor);
		printf("\n");
#endif

		int32_t nKey = (n >> nKeyShift) & 255;

		int32_t bit = lookupTableRadix16[dKey][nKey];

		int64_t sub = int64_t(bit) * divisor;

#ifdef _DEBUG
		printf("[DEBUG] nKey = ");
		PrintBinary(nKey);
		printf("\n");
		printf("[DEBUG] dKey = ");
		PrintBinary(dKey);
		printf("\n");

		printf("[DEBUG] bit = %u\n", bit);
		printf("[DEBUG] sub = %lli\n", sub);
		PrintBinary(sub);
		printf("\n");
#endif

		n -= sub;

#ifdef _DEBUG
		printf("[DEBUG] n after sub = %lli\n", n);
		PrintBinary(n);
		printf("\n");
#endif

		// Since the value from the lookup table uses
		// only the highest bits of numerator and divisor
		// to decide the multiplier, we might end up
		// subtracting too much. In this case it's necessary
		// to decrease multiplier by one to keep numerator
		// positive
		while (n < 0)
		{
#ifdef _DEBUG
			printf("[DEBUG] adjustment needed\n");
#endif
			n += divisor;
			bit--;

#ifdef _DEBUG
			printf("[DEBUG] n = %lli\n", n);
			PrintBinary(n);
			printf("\n");

			printf("[DEBUG] bit = %u\n", bit);
#endif
		}

		// Store 4 bits of result
		result = (result << 4) | bit;

#ifdef _DEBUG
		printf("[DEBUG] intermediate result = ");
		PrintBinary(result);
		printf("\n");
#endif
	}

#ifdef _DEBUG
	printf("[DEBUG] result = %u\n", result);
	PrintBinary(result);
	printf("\n");
	printf("[DEBUG] remainder = %llu\n", n);
	PrintBinary(n);
	printf("\n");
#endif

	* remainder = (uint32_t)n >> nBits;

	return result;
}