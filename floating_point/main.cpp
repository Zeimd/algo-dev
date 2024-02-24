#include <vector>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <ceng/lib/timerlib.h>

#include <ceng/datatypes/aligned-buffer.h>

#include "float16.h"

#include "binary_print.h"


int main()
{
	float x = -1.5f;

	PrintBinary(x);
	printf("\n");

	Float16 y(x);

	PrintBinary(y);
	printf("\n");

	float z = y;

	PrintBinary(z);
	printf("\n");

	return 0;
}
