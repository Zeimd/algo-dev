#ifndef FLOAT16_H
#define FLOAT16_H

#include <stdint.h>

#include "convert.h"

class Float16
{
public:
	uint16_t raw;

	

	Float16()
		: raw(0)
	{

	}

	Float16(float x)
	{
		raw = SingleToHalf(x);
	}

	operator float()
	{
		return HalfToSingle(raw);		
	}
};

#endif