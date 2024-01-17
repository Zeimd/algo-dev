#pragma once

void normalized_float_to_uint8(const float* input, unsigned char* output, const int inputSize);

void normalized_float_to_uint8_sse(const float* input, unsigned char* output, const int inputSize);

void normalized_float_to_uint8_optimized_sse(const float* input, unsigned char* output, const int inputSize);

void normalized_float_to_uint8_avx(const float* input, unsigned char* output, const int inputSize);

void uint8_to_normalized_float(const unsigned char* input, float* output, const int inputSize);

void uint8_to_normalized_float_strip4(const unsigned char* input, float* output, const int inputSize);

void uint8_to_normalized_float_strip8(const unsigned char* input, float* output, const int inputSize);

void uint8_to_normalized_float_strip16(const unsigned char* input, float* output, const int inputSize);

void uint8_to_normalized_float_sse(const unsigned char* input16, float* output16, const int inputSize);

void uint8_to_normalized_float_optimized_sse(const unsigned char* input16, float* output16, const int inputSize);

void uint8_to_normalized_float_sse_v3(const unsigned char* input16, float* output16, const int inputSize);

void uint8_to_normalized_float_avx(const unsigned char* input16, float* output16, const int inputSize);
