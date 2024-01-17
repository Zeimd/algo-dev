#include <intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <limits>
#include <cmath>

#include <ceng/lib/timerlib.h>

/* FastDiv by Jari Korkala , 20/8/2010
 * Uses the underlying idea of FastInvSqrt()
 * NOTE: Faster is quite subjective, at least
 *       on Intel Core2, where this is slower than FDIV.
 *       Must be the data transfer fpu->mem->reg->mem->fpu,
 *       which is not neccesary on ARM.
 */
inline float FastDiv(float x)
{

	float result;
	int temp;

	temp = *((int*)&x); // Use the floating point binary data as an integer

	/* The constant here does the following:
	 * (253 << 23) :
	 * When *temp* is substracted from an integer with
	 * this value in bits 23-30, it multiplies the exponent of the underlying
	 * floating point number by (-1), and substracts 1 from it,because we
	 * assume that the floating point number is not a power
	 * of two (there are only a few of those compared to the whole
	 * dynamic range).
	 *
	 * Remaining added bits (1 << x):
	 * Chosen so that the worst case relative error is as small as possible.
	 * Because mantissa bits closer to bit 23 are more important, the
	 * values can be found with a greedy algorithm that first sets each
	 * mantissa bit of *guessBase*, and then, starting from bit 22, tries
	 * one bit off at a time, and checks if the global error maxima over some
	 * suitably wide range of *x* drops.
	 */
	const int guessBase = (253 << 23) + (1 << 22) + (1 << 21) + (1 << 19) + (1 << 18)
		+ (1 << 17) + (1 << 16) + (1 << 15);

	// If the mantissa of *x* is larger than mantissa of *guessBase*,
	// the substraction borrows 1 from the exponent so that it will be correct.
	temp = guessBase - temp;

	result = *((float*)&temp);

	// One iteration of Newton-Raphson -method gives the result
	// that should hold up to 3 decimal places for any *x*.
	result = result * (2.0f - x * result);

	// Use more of these if extra precision is needed.
//	result = result*(2.0f-x*result);	

	return result;
}

void SpeedTest();
void AccuracyTest();

int main()
{
	AccuracyTest();

	return 0;
}

void SpeedTest()
{
	printf("SpeedTest\n");

	double start, end, refDuration;

	printf("native start\n");

	start = Ceng_HighPrecisionTimer();

	float acc = 0.0f;

	for (uint32_t rawBits = 0; rawBits < std::numeric_limits<uint32_t>::max(); rawBits++)
	{
		float divisor = *((float*)&rawBits);

		float val = 1.0f / divisor;

		acc += val;
	}

	end = Ceng_HighPrecisionTimer();

	printf("native end\n");

	printf("val = %lf\n", acc);

	refDuration = end - start;

	printf("refDuration = %lf , ratio = %lf\n", refDuration, refDuration / refDuration);

	printf("Fastdiv start\n");

	start = Ceng_HighPrecisionTimer();

	acc = 0.0f;

	for (uint32_t rawBits = 0; rawBits < std::numeric_limits<uint32_t>::max(); rawBits++)
	{
		float divisor = *((float*)&rawBits);

		float val = FastDiv(divisor);

		acc += val;
	}

	end = Ceng_HighPrecisionTimer();

	printf("Fastdiv end\n");


	double duration = end - start;

	printf("val = %lf\n", acc);
	printf("duration = %lf , ratio = %lf\n", duration, duration / refDuration);
}

void AccuracyTest()
{
	printf("AccuracyTest\n");

	float bestAbsErr = std::numeric_limits<float>::max();
	float worstAbsErr = 0.0f;

	float bestAbsInput = std::numeric_limits<float>::quiet_NaN();
	float worstAbsInput = std::numeric_limits<float>::quiet_NaN();

	float bestRelErr = std::numeric_limits<float>::max();
	float worstRelErr = 0.0f;

	float bestRelInput = std::numeric_limits<float>::quiet_NaN();
	float worstRelInput = std::numeric_limits<float>::quiet_NaN();

	float divisor = 0.000001f;
	float step = 0.0001f;
	float end = 100.0f;

	uint64_t iters = 0;
	int updateCounter = 0;

	double absErrTotal = 0.0;
	double relErrTotal = 0.0;

	do
	{
		float exact = 1.0f / divisor;

		float approx = FastDiv(divisor);

		float absErr = std::fabs(exact - approx);

		absErrTotal += absErr;

		if (absErr > worstAbsErr)
		{
			worstAbsErr = absErr;
			worstAbsInput = divisor;
		}

		if (absErr < bestAbsErr)
		{
			bestAbsErr = absErr;
			bestAbsInput = divisor;
		}

		float relErr = absErr / exact;	

		relErrTotal += relErr;

		if (relErr > worstRelErr)
		{
			worstRelErr = relErr;
			worstRelInput = divisor;
		}

		if (relErr < bestRelErr)
		{
			bestRelErr = relErr;
			bestRelInput = divisor;
		}

		divisor += step;
		iters++;
		updateCounter++;

		if (updateCounter % 10000)
		{
			updateCounter = 0;
			printf("divisor = %lf, absErr = %lf, relErr = %lf\n", divisor, absErr, relErr);
		}

	} while (divisor < end);

	double avgAbsErr = absErrTotal / double(iters);
	double avgRelErr = relErrTotal / double(iters);

	printf("best abs = %lf, worst abs = %lf\n", bestAbsErr, worstAbsErr);
	printf("best abs in = %lf, worst abs in = %lf\n", bestAbsInput, worstAbsInput);
	printf("best rel = %lf, worst rel = %lf\n", bestRelErr, worstRelErr);
	printf("best rel in = %lf, worst rel in = %lf\n", bestRelInput, worstRelInput);

	printf("avg abs err = %lf, avg rel err = %lf\n", avgAbsErr, avgRelErr);
}
