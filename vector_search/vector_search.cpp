#include "vector_search.h"

int find_reference(int* array, int length, int x)
{
	for (int k = 0; k < length; k++)
	{
		if (array[k] == x)
		{
			return k;
		}
	}

	return -1;
}

int find_group4(int* in_array, int in_length, int x)
{
	for (int k = 0; k < in_length; k+=4)
	{
		if (in_array[k] == x)
		{
			return k;
		}

		if (in_array[k+1] == x)
		{
			return k+1;
		}

		if (in_array[k + 2] == x)
		{
			return k + 2;
		}

		if (in_array[k + 3] == x)
		{
			return k + 3;
		}


	}

	return -1;
}

int find_sse(int* in_array, int in_length, int x)
{
	int result = -1;

	__asm
	{
		mov edx, in_length;
		mov esi, in_array;
		mov ecx, 0;

		movd xmm0, x;

		// xmm0 = {x,x,x,x}
		pshufd xmm0, xmm0, 0b;

	loop_start:

		cmp ecx, edx;
		jge end_loop;

		// xmm1 = {a3,a2,a1,a0}
		movdqa xmm1, [esi];

		// xmm1 : a[i] = -1 if a[i] == x, else 0
		pcmpeqd xmm1, xmm0;

		movmskps eax, xmm1;

		cmp eax, 0;
		jne finalize;

		add esi, 16; // address 
		add ecx, 4; // array index
		jmp loop_start;

	finalize:

		// *x* can appear only once in input, so EAX has exactly one bit set.
		// The position of this bit is the index we need. Conversion to float
		// makes this information easy to extract.

		movd xmm1, eax;
		cvtdq2ps xmm1, xmm1;
		psrld xmm1, 23;
		movd eax, xmm1;

		sub eax, 127;
		add eax, ecx;
		
		mov result, eax;
	end_loop:

	}

	return result;
}