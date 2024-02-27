#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "solid_angle.h"

// From AMD Cubemapgen
// https://code.google.com/archive/p/cubemapgen/
const float AreaElement(const float x, const float y)
{
	return atan2(x * y, sqrt(x * x + y * y + 1));
}

// From AMD Cubemapgen
// https://code.google.com/archive/p/cubemapgen/
const float TexelCoordSolidAngle(const float a_FaceIdx, const float a_U,
	const float a_V, const float a_Size)
{
	// transform from [0..res - 1] to [- (1 - 1 / res) .. (1 - 1 / res)]
	// (+ 0.5f is for texel center addressing)
	float U = (2.0f * ((float)a_U + 0.5f) / (float)a_Size) - 1.0f;
	float V = (2.0f * ((float)a_V + 0.5f) / (float)a_Size) - 1.0f;

	// Shift from a demi texel, mean 1.0f / a_Size with U and V in [-1..1]
	float InvResolution = 1.0f / a_Size;

	// U and V are the -1..1 texture coordinate on the current face.
	// Get projected area for this texel
	float x0 = U - InvResolution;
	float y0 = V - InvResolution;
	float x1 = U + InvResolution;
	float y1 = V + InvResolution;
	float SolidAngle = AreaElement(x0, y0) - AreaElement(x0, y1) - AreaElement(x1, y0) + AreaElement(x1, y1);

	return SolidAngle;
}

SolidAngleTable::SolidAngleTable()
	: width(0), data(nullptr)
{

}

SolidAngleTable::SolidAngleTable(uint32_t width)
	:width(width)
{
	data = (float*)malloc(width * width * sizeof(float));

	for (uint32_t v = 0; v < width; ++v)
	{
		for (uint32_t u = 0; u < width; ++u)
		{
			float* target = &data[v * width + u];

			*target = TexelCoordSolidAngle(0, float(u), float(v), width);
		}
	}
}

SolidAngleTable::~SolidAngleTable()
{
	if (data != nullptr)
	{
		free(data);
	}
}

float SolidAngleTable::Get(uint32_t u, uint32_t v)
{
	return data[width * v + u];
}

SolidAngleTableQuadrant::SolidAngleTableQuadrant(uint32_t width)
	: quadrantWidth(width >> 1)
	
{
	this->width = width;
	data = (float*)malloc(quadrantWidth * quadrantWidth * sizeof(float));

	for (uint32_t v = 0; v < quadrantWidth; ++v)
	{
		for (uint32_t u = 0; u < quadrantWidth; ++u)
		{
			float* target = &data[v * quadrantWidth + u];

			*target = TexelCoordSolidAngle(0, float(u), float(v), width);
		}
	}
}

SolidAngleTableQuadrant::~SolidAngleTableQuadrant()
{
	
}

float SolidAngleTableQuadrant::Get(uint32_t u, uint32_t v)
{
	uint32_t threshold = quadrantWidth - 1;
	uint32_t reflect = width - 1;

	uint32_t fetchU = u;
	if (fetchU > threshold)
	{
		fetchU = reflect - fetchU;
	}

	uint32_t fetchV = v;

	if (fetchV > threshold)
	{
		fetchV = reflect - fetchV;
	}

	/*
	printf(__func__);
	printf("(%i,%i) :\n", u, v);
	printf("threshold = %i, reflect = %i\n", threshold, reflect);
	printf("fetch = (%i,%i)\n", fetchU, fetchV);
	*/

	return data[fetchV * quadrantWidth + fetchU];
}