#include<stdio.h>
#include<limits.h>
#include<assert.h>
#include <stdbool.h>

/* Access bit-level representation floating-point number */
typedef unsigned float_bits;

/* Compute -f.  If f is NaN, then return f. */
float_bits float_negate(float_bits f) {
    /* Decompose bit representation into parts */
    unsigned sign = f>>31;
    unsigned exp = f>>23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    if (exp == 0xFF && frac != 0) {
        return f;
    }

    /* Reassemble bits */
    return ((~sign) << 31) | (exp << 23) | frac;
}

// used for testing
float to_float(float_bits x) {
    return *(float*) &x;
}

int main() {
    for (unsigned i = 0; i != UINT_MAX; i++) {
        float_bits fb = *(float_bits*) &i;
        float f = *(float*) &i;

        float negated_fb = to_float(float_negate(fb));

        bool is_nan = f != f; // my machine negates nan (changing the sign bit), but float_negate doesn't
        bool negated_is_non = negated_fb != negated_fb;
        assert((is_nan && negated_is_non) || negated_fb == -f); // either both nan or equal

        if (i % 42949673 == 0) printf("completed %.0f%%\n", (float) i / UINT_MAX * 100.0);
    }

    printf("done\n");
    return 0;
}