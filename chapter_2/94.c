#include<stdio.h>
#include<limits.h>
#include<assert.h>
#include<stdbool.h>

typedef unsigned float_bits;

/* Compute 2*f.  If f is NaN, then return f. */
float_bits float_twice(float_bits f) {
    unsigned exp = f>>23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned sign = f & 0x80000000;

    if (exp == 0xFF && frac != 0) { // nan
        return f;
    }

    if (exp == 0) {
        // denormalized
        frac = frac << 1;
        // one thing to note here, if there's a 1 right next to the binary point, after this left-shit, this 1 will slide to the exponent area.
        // this will effect in converting this number into a normalized one
        // M will now have and implicit 1 added to it (which is exactly like doubling this 1 after the)
        // for example, let's assume this number:
        // 0 00000000 10000000000000000000000 = M * 2^E = 1*2^-1 * 2^(1-127) = 2^-128   (E = 1 - Bias)
        // after the shift:
        // 0 00000001 00000000000000000000000 = M * 2^E = 1 * 2^(1-127) = 2^-127     (E = e - Bias)
    } else if (exp < 254) { // emax = Emax + Bias = 127 + 127 = 254
        // exponent isn't the max
        exp++;
    } else {
        return sign | 0b01111111100000000000000000000000; // ±∞
    }
    
    return sign | (exp << 23) | frac;
}

// used for testing
float to_float(float_bits x) {
    return *(float*) &x;
}

int main() {
    for (unsigned i = 0; i != UINT_MAX; i++) {
        float_bits fb = *(float_bits*) &i;
        float f = *(float*) &i;

        float twice_fb = to_float(float_twice(fb));

        bool is_nan = f != f;
        bool twice_is_non = twice_fb != twice_fb;
        assert((is_nan && twice_is_non) || twice_fb == 2*f); // either both nan or equal

        if (i % 42949672 == 0) printf("completed %.0f%%\n", (float) i / UINT_MAX * 100.0);
    }

    printf("done\n");

    return 0;
}