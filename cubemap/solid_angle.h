#ifndef SOLID_ANGLE_H
#define SOLID_ANGLE_H

#include <stdint.h>

class SolidAngleTable
{
protected:
	uint32_t width;

	float* data;

public:
	SolidAngleTable(uint32_t width);

	SolidAngleTable();

	~SolidAngleTable();

	virtual float Get(uint32_t u, uint32_t v);
};

class SolidAngleTableQuadrant : public SolidAngleTable
{
protected:
	uint32_t quadrantWidth;

public:

	SolidAngleTableQuadrant(uint32_t width);

	~SolidAngleTableQuadrant();

	float Get(uint32_t u, uint32_t v) override;
};


#endif