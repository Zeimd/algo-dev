Purposes:

1. Learning how things work
2. Vectorization and/or branch-free variants of algorithms
3. Finding faster approximations for use in game development
4. Testing silly ideas

--------------------------------------------------
celestial_nav

Too early to say what this will be used on, but probably involves solutions to spherical trigonometry
problems.

--------------------------------------------------
color_conversion

Using SSE intrinsics to convert between color formats.

--------------------------------------------------
cubemap

Testing ideas about how to speed up diffuse lighting convolution.

Plan is to move the exploration code from concept_engine repository here.

--------------------------------------------------
Division

Explores restoring division at varying radix. 

--------------------------------------------------
fast_div

Uses the Quake 3 fast inverse square root approach, but on 1/x. It's not very fast.

--------------------------------------------------
floating_point

Work towards conversion functions between float32 and float16.

--------------------------------------------------
modulus

Calculating x % 3 without doing a division.

--------------------------------------------------
trig

fast math approximations for transcendental functions. Intended to be used in shader code.

--------------------------------------------------
variadic_test

I wanted to try writing a C++ variadic template.

--------------------------------------------------
vector_search

Uses SSE intrinsics to check if given value is in an array.

--------------------------------------------------
vector_sort

Uses SSE intrinsics to form a sorting network for 4 input items.
