#pragma once

#include "trig.h"

constexpr float e_number = 2.71828182845904523536028747135266249f;

constexpr float log2e = 1.44269504088896f;

float exp_poly3(float x);

float exp2_poly3(float x);

// Calculate 2^x, where x is in range ]-1,1[
float exp2_principal_poly1(float x);

// Calculate 2^x, where x is in range ]-1,1[
float exp2_principal_poly2(float x);

// Calculate 2^x, where x is in range ]-1,1[
float exp2_principal_poly3(float x);

float log2_poly3(float x);

// Calculate log2(1+x), where x is in range [0,1[
float log2_principal_poly1(float x);

// Calculate log2(1+x), where x is in range [0,1[
float log2_principal_poly2(float x);

// Calculate log2(1+x), where x is in range [0,1[
float log2_principal_poly3(float x);
