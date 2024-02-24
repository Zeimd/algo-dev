#ifndef BINARY_PRINT_H
#define BINARY_PRINT_H

#include <stdint.h>

#include "float16.h"

void PrintBinary(const uint8_t x);

void PrintBinary(const uint8_t* x, const int32_t bytes);

void PrintBinary(const uint16_t x);
void PrintBinary(const uint32_t x);
void PrintBinary(const uint64_t x);

void PrintBinary(const int16_t x);
void PrintBinary(const int32_t x);
void PrintBinary(const int64_t x);

void PrintBinary(const float x);

void PrintBinary(const Float16 x);

#endif