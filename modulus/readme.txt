Divisibility by 3 check
------------------------------------------

The algorithm uses the fact that 

    2^n (mod 3) =
        1 (if n odd)
        2 (if n even)

so we can reduce an input number x0 to a smaller number x1 that will have same modulus:

    x0 (mod 3 ) = x1 (mod 3)

For an input of 32 bits, there are 16 even and 16 odd bits. This means that output of the first
reduction step is in range [0,48]. 

This is clearly too many values to check without a lookup table,
so a second reduction step (x2) can be used. By inspecting how these values behave in a second modular reduction,
it is obvious that it will be sufficient to check if x2 is 3 or 6:

    48 -> 3
    47 -> 8
    46 -> 7
    45 -> 6
    44 -> 5
    43 -> 5
    42 -> 6
    41 -> 5
    40 -> 3
    39 -> 6
    38 -> 5
    37 -> 5
    36 -> 3
    35 -> 5
    34 -> 4
    33 -> 3
    32 -> 2
    31 -> 7
    30 -> 6
    29 -> 5
    28 -> 4
    27 -> 6
    26 -> 5
    25 -> 4
    24 -> 3
    23 -> 5
    22 -> 4
    21 -> 3
    20 -> 2
    19 -> 4
    18 -> 3
    17 -> 2
    16 -> 1
    15 -> 6
    14 -> 5
    13 -> 4
    12 -> 3
    11 -> 4
    10 -> 4
    9 -> 3
    8 -> 2
    7 -> 4 
    6 -> 3
    5 -> 2
    4 -> 1
    3 -> 3
    2 -> 2
    1 -> 1

------------------------------------------------------------------------------------
Remainder when dividing by 3

The reduction steps can be continued until xN is in range [0,3]. At this point the return value is
given by

    if (xN == 3) return 0
    else return xN

