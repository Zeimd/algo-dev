
#include <algorithm>

#include <stdlib.h>
#include <string.h>

static int float_compare(const void* a, const void* b)
{
	float* x = (float*)a;
	float* y = (float*)b;

	if (*x < *y)
	{
		return -1;
	}
	else if (*x > *y)
	{
		return 1;
	}

	return 0;
}

void float_sort4_insertion_sort(float* input4, const int size)
{
	for (int group = 0; group < size; group += 4)
	{
		for (int i = 1; i < 4; i++)
		{
			float x = input4[group + i];

			int j = i - 1;

			while (j >= 0 && input4[group + j] > x)
			{
				input4[group + j + 1] = input4[group + j];
				--j;
			};

			input4[group + j + 1] = x;
		}
	}
}

void float_sort4_selection_sort(float* input4, const int size)
{
	for (int group = 0; group < size; group += 4)
	{
		for (int i = 0; i < 4; i++)
		{
			int smallIndex = i;
			float smallVal = input4[group + i];

			for (int j = smallIndex+1; j < 4; j++)
			{
				float val = input4[group + j];

				if (val < smallVal)
				{
					smallIndex = j;
					smallVal = val;
				}
			}

			float temp = input4[group + i];
			input4[group + i] = smallVal;
			input4[group + smallIndex] = temp;
		}		
	}
}


void float_sort4_stdlib_qsort(float* input4, const int size)
{
	for (int k = 0; k < size; k += 4)
	{
		qsort(&input4[k], 4, sizeof(float), &float_compare);
	}
}

void float_sort4_minmax_stl(float* input4, const int size)
{
	for (int group = 0; group < size; group += 4)
	{
		float a = input4[group];
		float b = input4[group + 1];
		float c = input4[group + 2];
		float d = input4[group + 3];

		float t0 = std::min(a, b);
		float t1 = std::min(c, d);
		float t2 = std::max(a, b);
		float t3 = std::max(c, d);

		float t4 = std::max(t0, t1);
		float t5 = std::min(t2, t3);

		input4[group] = std::min(t0, t1);
		input4[group + 1] = std::min(t4, t5);
		input4[group + 2] = std::max(t4, t5);
		input4[group + 3] = std::max(t2, t3);
	}
}

void float_sort4_minmax_stl_int_punning(float* input4, const int size)
{
	for (int group = 0; group < size; group += 4)
	{
		int *a = (int*)&input4[group];
		int *b = (int*)&input4[group + 1];
		int *c = (int*)&input4[group + 2];
		int *d = (int*)&input4[group + 3];

		int t0 = std::min(*a, *b);
		int t1 = std::min(*c, *d);

		int t2 = std::max(*a, *b);
		int t3 = std::max(*c, *d);

		int t4 = std::max(t0, t1);
		int t5 = std::min(t2, t3);

		*a = std::min(t0, t1);
		*b = std::min(t4, t5);
		*c = std::max(t4, t5);
		*d = std::max(t2, t3);
	}
}

void float_sort4_minmax_x87_cmov(float* input4, const int size)
{
	for (int group = 0; group < size; group += 4)
	{
		float *start = &input4[group];

		float abMin, abMax;
		float cdMin, cdMax;

		float minOfMax, maxOfMin;

		__asm
		{
			mov esi, start;

			//-------------------------------
			// Calculate min(a,b) and max(a,b)

			fld DWORD PTR [esi];
			fld DWORD PTR [esi + 4]; 
			
			// st(0) = b
			// st(1) = a

			fucomi st, st(1); // b > a ?

			fld st(0);

			// st(0) = b
			// st(1) = b
			// st(2) = a

			fcmovnb st(0), st(2);

			// st(0) = min(a,b)
			// st(1) = b
			// st(2) = a

			fstp DWORD PTR[abMin];

			// st(1) = b
			// st(2) = a

			fcmovb st(0), st(1);

			// st(0) = max(a,b)
			// st(1) = a

			fstp DWORD PTR [abMax];
			fstp st(0);

			//-------------------------------
			// Calculate min(c,d) and max(c,d)

			fld DWORD PTR[esi+8];
			fld DWORD PTR[esi + 12];

			// st(0) = d
			// st(1) = c

			fucomi st, st(1); // d > c ?

			fld st(0);

			// st(0) = d
			// st(1) = d
			// st(2) = c

			fcmovnb st(0), st(2);

			// st(0) = min(c,d)
			// st(1) = d
			// st(2) = c

			fstp DWORD PTR[cdMin];

			// st(1) = d
			// st(2) = c

			fcmovb st(0), st(1);

			// st(0) = max(c,d)
			// st(1) = c

			fstp DWORD PTR[cdMax];
			fstp st(0);

			//-------------------------------
			// Calculate min(abMin,cdMin) and max(abMin,cdMin)

			fld DWORD PTR[abMin];
			fld DWORD PTR[cdMin];

			// st(0) = cdMin
			// st(1) = abMin

			fucomi st, st(1); // cdMin > abMin ?

			fld st(0);

			// st(0) = cdMin
			// st(1) = cdMin
			// st(2) = abMin

			fcmovnb st(0), st(2);

			// st(0) = min(abMin,cdMin)
			// st(1) = cdMin
			// st(2) = abMin

			fstp DWORD PTR[esi];  // out[0]

			// st(1) = cdMin
			// st(2) = abMin

			fcmovb st(0), st(1);

			// st(0) = max(abMin,cdMin)
			// st(1) = abMin

			fstp DWORD PTR[maxOfMin];
			fstp st(0);

			//-------------------------------
			// Calculate min(abMax,cdMax) and max(abMax,cdMax)

			fld DWORD PTR[abMax];
			fld DWORD PTR[cdMax];

			// st(0) = cdMax
			// st(1) = abMax

			fucomi st, st(1); // cdMax > abMax ?

			fld st(0);

			// st(0) = cdMax
			// st(1) = cdMax
			// st(2) = abMax

			fcmovnb st(0), st(2);

			// st(0) = min(abMax,cdMax)
			// st(1) = cdMax
			// st(2) = abMax

			fstp DWORD PTR[minOfMax];

			// st(1) = cdMax
			// st(2) = abMax

			fcmovb st(0), st(1);

			// st(0) = max(abMax,cdMax)
			// st(1) = abMax

			fstp DWORD PTR[esi+12]; // out[3]
			fstp st(0);

			//-------------------------------
			// Calculate min(minOfMax,maxOfMin) and
			// max(minOfMax,maxOfMin)

			fld DWORD PTR[minOfMax];
			fld DWORD PTR[maxOfMin];

			// st(0) = maxOfMin
			// st(1) = minOfMax

			fucomi st, st(1); // maxOfMin > minOfMax ?

			fld st(0);

			// st(0) = maxOfMin
			// st(1) = maxOfMin
			// st(2) = minOfMax

			fcmovnb st(0), st(2);

			// st(0) = min(minOfMax,maxOfMin)
			// st(1) = maxOfMin
			// st(2) = minOfMax

			fstp DWORD PTR[esi+4]; // out[1]

			// st(1) = maxOfMin
			// st(2) = minOfMax

			fcmovb st(0), st(1);

			// st(0) = max(minOfMax,maxOfMin)
			// st(1) = minOfMax

			fstp DWORD PTR[esi + 8]; // out[2]
			fstp st(0);
		}
	}
}

void float_sort4_minmax_x87_cmov_v2(float* input4, const int size)
{
	for (int group = 0; group < size; group += 4)
	{
		float* start = &input4[group];

		__asm
		{
			mov esi, start;

			//-------------------------------
			// Calculate min(a,b) and max(a,b)

			fld DWORD PTR[esi];
			fld DWORD PTR[esi + 4];

			// st(0) = b
			// st(1) = a

			fucomi st, st(1); // b > a ?

			fld st(0);

			// st(0) = b
			// st(1) = b
			// st(2) = a

			fcmovnb st(0), st(2);

			// st(0) = min(a,b)
			// st(1) = b
			// st(2) = a

			fxch st(1);

			// st(0) = b
			// st(1) = min(a,b)
			// st(2) = a

			fcmovb st(0), st(2);

			// st(0) = max(a,b)
			// st(1) = min(a,b)
			// st(2) = a

			fstp st(2);

			// st(0) = min(a,b)
			// st(1) = max(a,b)

			//-------------------------------
			// Calculate min(c,d) and max(c,d)

			fld DWORD PTR[esi + 8];
			fld DWORD PTR[esi + 12];

			// st(0) = d
			// st(1) = c
			// st(0) = min(a,b)
			// st(1) = max(a,b)

			fucomi st, st(1); // d > c ?

			fld st(0);

			// st(0) = d
			// st(1) = d
			// st(2) = c
			// st(3) = min(a,b)
			// st(4) = max(a,b)

			fcmovnb st(0), st(2);

			// st(0) = min(c,d)
			// st(1) = d
			// st(2) = c
			// st(3) = min(a,b)
			// st(4) = max(a,b)

			fxch st(1);

			// st(0) = d
			// st(1) = min(c,d)
			// st(2) = c
			// st(3) = min(a,b)
			// st(4) = max(a,b)

			fcmovb st(0), st(2);

			// st(0) = max(c,d)
			// st(1) = min(c,d)
			// st(2) = c
			// st(3) = min(a,b)
			// st(4) = max(a,b)

			fstp st(2);

			// st(0) = cdMin
			// st(1) = cdMax 
			// st(2) = abMin
			// st(3) = abMax

			//-------------------------------
			// Calculate min(abMin,cdMin) and max(abMin,cdMin)

			fucomi st, st(2); // cdMin > abMin ?

			fld st(0);

			// st(0) = cdMin
			// st(1) = cdMin
			// st(2) = cdMax 
			// st(3) = abMin
			// st(4) = abMax

			fcmovnb st(0), st(3);

			// st(0) = min(abMin,cdMin)
			// st(1) = cdMin
			// st(2) = cdMax 
			// st(3) = abMin
			// st(4) = abMax

			fxch st(1);

			// st(0) = cdMin
			// st(1) = min(abMin,cdMin)
			// st(2) = cdMax 
			// st(3) = abMin
			// st(4) = abMax

			fcmovb st(0), st(3);

			fstp st(3);

			// st(0) = min(abMin,cdMin) = out[0]
			// st(1) = cdMax 
			// st(2) = max(abMin,cdMin) = maxOfMin
			// st(3) = abMax

			//-------------------------------
			// Calculate min(abMax,cdMax) and max(abMax,cdMax)

			fxch st(1);

			// st(0) = cdMax 
			// st(1) = min(abMin,cdMin) = out[0]
			// st(2) = max(abMin,cdMin) = maxOfMin
			// st(3) = abMax

			fucomi st, st(3); // cdMax > abMax ?

			fld st(0);

			// st(0) = cdMax 
			// st(1) = cdMax 
			// st(2) = min(abMin,cdMin) = out[0]
			// st(3) = max(abMin,cdMin) = maxOfMin
			// st(4) = abMax

			fcmovnb st(0), st(4);

			// st(0) = min(abMax,cdMax) = minOfMax
			// st(1) = cdMax 
			// st(2) = min(abMin,cdMin) = out[0]
			// st(3) = max(abMin,cdMin) = maxOfMin
			// st(4) = abMax

			fxch st(1);

			// st(0) = cdMax 
			// st(1) = min(abMax,cdMax) = minOfMax
			// st(2) = min(abMin,cdMin) = out[0]
			// st(3) = max(abMin,cdMin) = maxOfMin
			// st(4) = abMax

			fcmovb st(0), st(4);

			// st(0) = max(abMax,cdMax) = out[3]
			// st(1) = min(abMax,cdMax) = minOfMax
			// st(2) = min(abMin,cdMin) = out[0]
			// st(3) = max(abMin,cdMin) = maxOfMin
			// st(4) = abMax

			//-------------------------------
			// Calculate min(abMax,cdMax) and max(abMax,cdMax)

			fxch st(1);

			// st(0) = minOfMax
			// st(1) = out[3]
			// st(2) = out[0]
			// st(3) = maxOfMin
			// st(4) = abMax

			fucomi st, st(3); // minOfMax > maxOfMin ?

			fld st(0);

			// st(0) = minOfMax
			// st(1) = minOfMax
			// st(2) = out[3]
			// st(3) = out[0]
			// st(4) = maxOfMin
			// st(5) = abMax

			fcmovnb st(0), st(4);

			// st(0) = out[1]
			// st(1) = minOfMax
			// st(2) = out[3]
			// st(3) = out[0]
			// st(4) = maxOfMin
			// st(5) = abMax

			fxch st(1);

			// st(0) = minOfMax
			// st(1) = out[1]
			// st(2) = out[3]
			// st(3) = out[0]
			// st(4) = maxOfMin
			// st(5) = abMax

			fcmovb st(0), st(4);

			// st(0) = out[2]
			// st(1) = out[1]
			// st(2) = out[3]
			// st(3) = out[0]
			// st(4) = maxOfMin
			// st(5) = abMax

			fxch st(3);

			// st(0) = out[0]
			// st(1) = out[1]
			// st(2) = out[3]
			// st(3) = out[2]
			// st(4) = maxOfMin
			// st(5) = abMax

			fstp[esi];

			// st(0) = out[1]
			// st(1) = out[3]
			// st(2) = out[2]
			// st(3) = maxOfMin
			// st(4) = abMax

			fstp[esi+4];

			// st(0) = out[3]
			// st(1) = out[2]
			// st(2) = maxOfMin
			// st(3) = abMax

			fxch st(1);

			// st(0) = out[2]
			// st(1) = out[3]
			// st(2) = maxOfMin
			// st(3) = abMax

			fstp[esi + 8];

			// st(0) = out[3]
			// st(1) = maxOfMin
			// st(2) = abMax

			fstp[esi + 12];
			fstp st(0);
			fstp st(0);


		}
	}
}

void float_sort4_minmax_sse_scalar(float* input4, const int size)
{
	for (int k = 0; k < size; k += 4)
	{
		__asm
		{
			mov esi, input4;
			mov eax, k;
			shl eax, 2;

			vmovss xmm0, [esi + eax];
			vmovss xmm1, [esi + eax + 4];
			vmovss xmm2, [esi + eax + 8];
			vmovss xmm3, [esi + eax + 12];

			// min(a,b)
			vminss xmm4, xmm0, xmm1;

			// min(c,d)
			vminss xmm5, xmm2, xmm3;

			// max(a,b)
			vmaxss xmm0, xmm0, xmm1;

			// max(c,d)
			vmaxss xmm1, xmm2, xmm3;

			// out[0]
			vminss xmm2, xmm4, xmm5;

			// out[3]
			vmaxss xmm3, xmm0, xmm1;

			// maxOfMin
			vmaxss xmm4, xmm4, xmm5;

			// minOfMax
			vminss xmm0, xmm0, xmm1;

			// out[1]
			vminss xmm1, xmm0, xmm4;

			// out[2]
			vmaxss xmm0, xmm0, xmm4;

			vmovss[esi + eax], xmm2;
			vmovss[esi + eax + 4], xmm1;
			vmovss[esi + eax + 8], xmm0;
			vmovss[esi + eax + 12], xmm3;

		}
	}
}

// Vectorized version of minmax sort
void float_sort4_minmax_sse(float* input4, const int size)
{
	// 19 instructions

	for (int k = 0; k < size; k += 4)
	{
		__asm
		{
			mov esi, input4;
			mov eax, k;
			shl eax, 2;

			vmovdqa xmm0, [esi + eax];

			vmovdqa xmm1, xmm0;

			// xmm0 = [d, c, b, a]

			vpsrldq xmm1, xmm1, 8;

			// xmm1 = [0, 0, d, c]

			vmovaps xmm2, xmm0;
			vminps xmm0, xmm0, xmm1;
			vmaxps xmm2, xmm2, xmm1;

			// xmm0 = [x, x, bdMin, acMin]
			// xmm1 = [x, x, bdMax, acMax]

			vmovaps xmm1, xmm0;
			vunpcklps xmm1, xmm1, xmm2;

			// xmm1 = [bdMax, bdMin, acMax, acMin]

			vpshufd xmm3, xmm1, 01001110b;

			// xmm3 = [acMax, acMin, bdMax, bdMin]

			vmovaps xmm0, xmm1;
			vminps xmm0, xmm0, xmm3;

			// xmm0 = [minOfMax, out[0], minOfMax, out[0]]

			vmaxps xmm3, xmm3, xmm1;

			// xmm3 = [out[3], maxOfMin, out[3], maxOfMin]

			vmovaps xmm2, xmm0;

			vpsrldq xmm2, xmm2, 4;

			// xmm2 = [0, minOfMax, out[0], minOfMax]

			vmovaps xmm1, xmm2;
			
			vminps xmm1, xmm1, xmm3;
			vmaxps xmm2, xmm2, xmm3;

			// xmm1 = [x, out[1], out[0], out[1] ]

			// xmm2 = [x, out[2], out[3], out[2] ]

			vshufps xmm1, xmm1, xmm2, 01000001b;

			// xmm1 = [out[3], out[2], out[1], out[0] ]

			vmovaps[esi + eax], xmm1;			
		}
	}
}

__declspec(align(16)) static const uint32_t flipMask1[4] = {0, 0, uint32_t(1) << 31, uint32_t(1) << 31};
__declspec(align(16)) static const uint32_t flipMask2[4] = {0, uint32_t(1) << 31, 0, uint32_t(1) << 31};
__declspec(align(16)) static const uint32_t flipMask3[4] = {0, 0, uint32_t(1) << 31, 0 };


// Alternate version
void float_sort4_minmax_sse_v2(float* input4, const int size)
{
	// 20 instructions
	// 3 constants loaded from memory

	for (int k = 0; k < size; k += 4)
	{
		__asm
		{
			mov esi, input4;
			mov eax, k;
			shl eax, 2;

			vmovdqa xmm0, [esi + eax];
			
			// xmm0 = [d, c, b, a]

			vpshufd xmm1, xmm0, 01001110b;

			// xmm1 = [b, a, d, c]

			vmovdqa xmm2, [flipMask1];

			vxorps xmm0, xmm0, xmm2;
			vxorps xmm1, xmm1, xmm2;

			// xmm0 = [-d, -c, b, a]
			// xmm1 = [-b, -a, d, c]

			vminps xmm0, xmm0, xmm1;
	
			// xmm0 = [min(-d,-b), min(-c,-a), min(b,d), min(a,c) ]

			vxorps xmm0, xmm0, xmm2;

			// xmm0 = [max(b,d), max(a,c), min(b,d), min(a,c) ]

			pshufd xmm1, xmm0, 10110001b;

			// xmm1 = [max(a,c), max(b,d), min(a,c), min(b,d) ]

			vmovaps xmm2, [flipMask2];

			vxorps xmm0, xmm0, xmm2;
			vxorps xmm1, xmm1, xmm2;

			// xmm0 = [-bdMax, acMax, -bdMin, acMin ]
			
			// xmm1 = [-acMax, bdMax, -acMin, bdMin ]

			vminps xmm0, xmm0, xmm1;

			// xmm0 = [min(-acMax,-bdMax), min(acMax, bdMax), min(-acMin, -bdMin), min(acMin, bdMin)]

			vxorps xmm0, xmm0, xmm2;

			// xmm0 = [max(acMax,bdMax), min(acMax, bdMax), max(acMin, bdMin), min(acMin, bdMin)]
			//      = [out[3], minOfMax, maxOfMin, out[0] ]

			vpshufd xmm1, xmm0, 11011000b;
	
			// xmm1 = [out[3], maxOfMin, minOfMax, out[0] ]

			vmovaps xmm2, [flipMask3];

			vxorps xmm0, xmm0, xmm2;
			vxorps xmm1, xmm1, xmm2;

			// xmm0 = [out[3], -minOfMax, maxOfMin, out[0] ]

			// xmm1 = [out[3], -maxOfMin, minOfMax, out[0] ]

			vminps xmm0, xmm0, xmm1;

			// xmm1 = [out[3], min(-minOfMax,-maxOfMin), min(minOfMax,maxOfMin), out[0] ]

			vxorps xmm0, xmm0, xmm2;
			

			vmovaps[esi + eax], xmm0;
			
		}
	}
}