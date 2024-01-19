#pragma once

#include "trig.h"

constexpr float e_number = 2.71828182845904523536028747135266249f;

constexpr float log2_of_e  = 1.44269504088896f;
constexpr float log2_of_10 = 3.32192809488736f;

float pow_poly3(float base, float x);

float exp_poly3(float x);

float exp2_poly3(float x);

// Calculate 2^x, where x is in range ]-1,1[
float exp2_principal_poly1(float x);

// Calculate 2^x, where x is in range ]-1,1[
float exp2_principal_poly2(float x);

// Calculate 2^x, where x is in range ]-1,1[
float exp2_principal_poly3(float x);

// Natural logarithm
float log_poly3(float x);

// Base 10 logarithm
float log10_poly3(float x);

// Base 2 logarithm
float log2_poly3(float x);

// Calculate log2(1+x), where x is in range [0,1[
float log2_principal_poly1(float x);

// Calculate log2(1+x), where x is in range [0,1[
float log2_principal_poly2(float x);

// Calculate log2(1+x), where x is in range [0,1[
float log2_principal_poly3(float x);
