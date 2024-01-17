#include <iostream>
#include <cmath>
#include <cstdlib>

#include <ceng/lib/timerlib.h>

#include "color_conversion.h"

#include <ceng/datatypes/aligned-buffer.h>

void round_trip_test(const unsigned char* input);

void accuracy_test(float* exact, float* test, const int size);

int sort_test();

int uint8_to_normalized_float_tests();

int main()
{
	uint8_to_normalized_float_tests();

	return 0;
}

typedef void (*uint8_to_normalized_float_callback)(const unsigned char* input, float* output, const int inputSize);

double uint8_to_normalized_float_speed_test(const char* name, uint8_to_normalized_float_callback callback,
	const unsigned char* input, float* output, const int inputSize, double base_duration)
{
	double start_time, end_time;
	double duration;

	start_time = Ceng_HighPrecisionTimer();

	(*callback)(input, output, inputSize);

	end_time = Ceng_HighPrecisionTimer();

	duration = end_time - start_time;

	std::cout << name << ": " << duration << " , ratio = " << duration / base_duration << std::endl;

	return duration;
}

typedef void (*normalized_float_to_uint8_callback)(const float* input, unsigned char* output, const int inputSsize);

double normalized_float_to_uint8_speed_test(const char* name, normalized_float_to_uint8_callback callback,
	const float* input, unsigned char* output, const int inputSize, double base_duration)
{
	double start_time, end_time;
	double duration;

	start_time = Ceng_HighPrecisionTimer();

	(*callback)(input, output, inputSize);

	end_time = Ceng_HighPrecisionTimer();

	duration = end_time - start_time;

	std::cout << name << ": " << duration << " , ratio = " << duration / base_duration << std::endl;

	return duration;
}

int uint8_to_normalized_float_tests()
{
	_declspec(align(32)) unsigned char input_sse[256];

	for (int k = 0; k < 256; ++k)
	{
		input_sse[k] = unsigned char(k);
	}

	float results[256];

	std::cout << "C version:" << std::endl;

	uint8_to_normalized_float(input_sse, results, 256);

	_declspec(align(32)) unsigned char round_trip[256];

	normalized_float_to_uint8(results, round_trip, 256);

	//round_trip_test(round_trip);

	//_declspec(align(32)) unsigned char sse_round_trip[256];

	// SSE default version

	/*


	std::cout << "default sse version:" << std::endl;

	_declspec(align(16)) float results_default_sse[256];

	uint8_to_normalized_float_sse(input_sse, results_default_sse,256);

	//accuracy_test(results, results_default_sse,256);

	normalized_float_to_uint8_sse(results_default_sse, sse_round_trip, 256);

	//round_trip_test(sse_round_trip);

	std::cout << "*******************************************************" << std::endl;
	std::cout << "optimized sse version:" << std::endl;

	_declspec(align(16)) float results_optimized_sse[256];

	uint8_to_normalized_float_optimized_sse(input_sse, results_optimized_sse, 256);

	//accuracy_test(results, results_optimized_sse, 256);

	normalized_float_to_uint8_optimized_sse(results_optimized_sse, sse_round_trip, 256);
	*/
	//round_trip_test(sse_round_trip);

	//_declspec(align(32)) float results_avx[256];

	/*
	uint8_to_normalized_float_avx(input_sse, results_avx, 256);

	accuracy_test(results, results_avx, 256);

	normalized_float_to_uint8_avx(results_avx, sse_round_trip, 256);

	round_trip_test(sse_round_trip);
	*/
	//return 0;

	double base_duration;

	const int test_size = 16 * 10000000;

	float* speed_input = (float*)Ceng::AlignedMalloc(test_size * sizeof(float), 64);

	unsigned char* speed_result = (unsigned char*)Ceng::AlignedMalloc(test_size * sizeof(unsigned char), 64);

	std::cout << "randomizing input" << std::endl;
	for (int k = 0; k < test_size; ++k)
	{
		speed_input[k] = (rand() % 257) / 256.0f;
		speed_result[k] = 0;
	}

	std::cout << "*******************************************************" << std::endl;
	std::cout << "speed test: normalized float -> uint8" << std::endl;

	base_duration = normalized_float_to_uint8_speed_test("Scalar C", normalized_float_to_uint8, speed_input, speed_result, test_size, 1.0);

	normalized_float_to_uint8_speed_test("SSE v1", normalized_float_to_uint8_sse, speed_input, speed_result, test_size, base_duration);
	normalized_float_to_uint8_speed_test("SSE v2", normalized_float_to_uint8_optimized_sse, speed_input, speed_result, test_size, base_duration);
	normalized_float_to_uint8_speed_test("AVX", normalized_float_to_uint8_avx, speed_input, speed_result, test_size, base_duration);

	std::cout << "*******************************************************" << std::endl;
	std::cout << "speed test: uint8 -> normalized float" << std::endl;

	base_duration = uint8_to_normalized_float_speed_test("Scalar C", uint8_to_normalized_float, speed_result, speed_input, test_size, 1.0);

	uint8_to_normalized_float_speed_test("scalar , 4x strip", uint8_to_normalized_float_strip4, speed_result, speed_input, test_size, base_duration);
	uint8_to_normalized_float_speed_test("scalar , 8x strip", uint8_to_normalized_float_strip8, speed_result, speed_input, test_size, base_duration);
	uint8_to_normalized_float_speed_test("scalar , 16x strip", uint8_to_normalized_float_strip16, speed_result, speed_input, test_size, base_duration);

	uint8_to_normalized_float_speed_test("SSE_v1", uint8_to_normalized_float_sse, speed_result, speed_input, test_size, base_duration);
	uint8_to_normalized_float_speed_test("SSE_v2", uint8_to_normalized_float_optimized_sse, speed_result, speed_input, test_size, base_duration);
	uint8_to_normalized_float_speed_test("SSE_v3", uint8_to_normalized_float_sse_v3, speed_result, speed_input, test_size, base_duration);
	uint8_to_normalized_float_speed_test("AVX", uint8_to_normalized_float_avx, speed_result, speed_input, test_size, base_duration);

	Ceng::AlignedFree(speed_input);
	Ceng::AlignedFree(speed_result);

	return 0;
}
