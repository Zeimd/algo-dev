#include <stdio.h>

#include "tuple_test.h"

int main()
{
	//TestTuple<int, double, float> a;
	TestTuple<int, double, float> a{ 14,7.0,3.141f };

	printf("tuple length = %i\n", a.length);
	//printf("tuple length, struct template = %i\n", a.length_from_struct_template);

	/*
	a.item.value = 14;
	a.item.item.value = 7.0;
	a.item.item.item.value = 3.141f;
	*/

	//a.Set<2>(6.22f);
	
	printf("a.int = %i\n", a.Get<0>());

	printf("a.double = %lf\n", a.Get<1>());
	printf("a.float = %lf\n", a.Get<2>());

	return 0;
}