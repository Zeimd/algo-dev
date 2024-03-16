#include <stdio.h>

#include "tuple_test.h"

int main()
{
	TestTuple<int, double> a;

	a.item.value = 14;

	printf("a.int = %i\n", a.item.value);

	printf("a.int = %i\n", a.Get<1>());

	return 0;
}