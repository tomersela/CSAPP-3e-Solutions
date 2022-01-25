#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
#include<assert.h>

typedef unsigned float_bits;

/*
* Compute (int) f.
* If conversion causes overflow or f is NaN, return 0x80000000
*/
int float_f2i(float_bits f) {
    unsigned exp = f>>23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned sign = f & 0x80000000;

    const int bias = 127;
    int exponent = exp - bias;

    if (exponent > 30) { // nan or out of range
        return 0x80000000; // -0
    }

    if (exponent < 0) { // smaller than 1
        return 0; // return 0 while keeping the sign
    }

    const implicit_one = 0x00800000;
    unsigned n = ((frac | implicit_one) << 8) >> (31 - exponent);
    return sign ? -n : n;
}

int main() {
    for (unsigned i = 0; i != UINT_MAX; i++) {
        float_bits fb = *(float_bits*) &i;
        float f = *(float*) &i;

        int converted = float_f2i(fb);
        int casted = (int) f;

        if (i % 42949672 == 0) printf("completed %.0f%%\n", (float) i / UINT_MAX * 100.0);
    }

    printf("done\n");

    return 0;
}