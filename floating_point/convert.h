#ifndef CONVERT_H
#define CONVERT_H

#include <stdint.h>

const uint16_t halfPlusInfinity = (31 << 10);
const uint16_t halfMinusInfinity = (31 << 10) | (1 << 15);

const uint16_t halfSignalingNan = (31 << 10) | (1 << 8);
const uint16_t halfQuietNan = (31 << 10) | (1 << 9) | (1 << 8);

const uint32_t singleMantissaMask = (1 << 23) - 1;
const uint32_t singleExponentMask = uint32_t(255) << 23;
const uint32_t singleSignMask = uint32_t(1) << 31;

static const float halfMax = 65504.0f;
static const float halfMin = 1.0f / (float(1 << 24));

uint16_t SingleToHalf(float x);

float HalfToSingle(uint16_t x);


#endif