#include<stdio.h>
#include<limits.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

typedef unsigned float_bits;

/* Compute |f|.  If f is NaN, then return f. */
float_bits float_absval(float_bits f) {
    unsigned exp = f>>23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    if (exp == 0xFF && frac != 0) {
        return f;
    }

    unsigned mask = 0x7FFFFFFF;
    return f & mask;
}

// used for testing
float to_float(float_bits x) {
    return *(float*) &x;
}

int main() {
    for (unsigned i = 0; i != UINT_MAX; i++) {
        float_bits fb = *(float_bits*) &i;
        float f = *(float*) &i;

        float abs_fb = to_float(float_absval(fb));

        bool is_nan = f != f; // my machine negates nan (changing the sign bit), but float_negate doesn't
        bool abs_is_non = abs_fb != abs_fb;
        assert((is_nan && abs_is_non) || abs_fb == fabs(f)); // either both nan or equal
        
        if (i % 42949673 == 0) printf("completed %.0f%%\n", (float) i / UINT_MAX * 100.0);
    }

    printf("done\n");

    return 0;
}