#include<stdio.h>
#include<limits.h>
#include<assert.h>
#include<stdbool.h>

typedef unsigned float_bits;

/* Compute 0.5*f.  If f is NaN, then return f. */
float_bits float_half(float_bits f) {
    unsigned exp = f>>23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned sign = f & 0x80000000;

    if (exp == 0xFF && frac == 0) { // ±∞
        return f;
    }

    if (exp == 0) { // denormalized or 0
        int rounding = (frac & 0x3) == 0x3;
        frac = (frac >> 1) + rounding;
    } else { // normalized
        exp--;
        if (exp == 0) { // normalized to denormalized
            int rounding = (frac & 0x3) == 0x3;
            frac = ((frac >> 1) | 0x400000) + rounding; // 0x400000 is for adding a 1 right after the binary point
        }
    }

    // printf("sign | (exp << 23) | frac = %08x\n", sign | (exp << 23) | frac);
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

        float half_fb = to_float(float_half(fb));
        float half = f/2;

        bool is_nan = f != f;
        bool half_fb_nan = f != f;

        assert((is_nan && half_fb_nan) || half_fb == f/2);

        if (i % 42949672 == 0) printf("completed %.0f%%\n", (float) i / UINT_MAX * 100.0);
    }

    printf("done\n");
    return 0;
}