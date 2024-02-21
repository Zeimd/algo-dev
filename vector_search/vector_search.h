#pragma once

typedef int(*SearchHandle)(int* array, int length, int x);

int find_reference(int* array, int length, int x);

// Check 4 entries per iteration
int find_group4(int* in_array, int in_length, int x);

int find_sse(int* array, int length, int x);