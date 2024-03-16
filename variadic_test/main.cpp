#include <stdio.h>

#include "tuple_test.h"

int main()
{
	TestTuple<int, double,float> a;

	a.item.value = 14;
	a.item.item.value = 7.0;

	printf("a.int = %i\n", a.item.value);
	printf("a.int = %i\n", a.Get<0>());

	printf("a.int = %lf\n", a.Get<1>());
	printf("a.int = %lf\n", a.Get<2>());

	return 0;
}