#pragma once

typedef int(*SearchHandle)(int* array, int length, int x);

int find_reference(int* array, int length, int x);

int find_sse(int* array, int length, int x);