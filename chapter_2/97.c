#include<stdio.h>
#include<limits.h>
#include<assert.h>

typedef unsigned float_bits;
    
const int bias = 127;

int frac_length(unsigned n) {
    unsigned temp = n;
    int exponent = 0;
    while (temp > 0) {
        temp >>= 1;
        exponent++;
    }
    return exponent - 1; // not considering the implicit most significant 1
}

/* Compute (float) i */
float_bits float_i2f(int i) {
    if (i == 0) {
        return 0;
    }

    unsigned sign = i & 0x80000000;
    int n = sign ? -i : i;
    int exp = frac_length(n);
    
    unsigned frac = n << (32 - exp); // remove the implicit 1
    // we now have 32 bit fraction (which we'll reduce later to 23 bit)

    // round to even
    // in case our number won't fit into the 23 bit fraction precision, we need to round it
    // casting from int to float is using round-to-even rounding, so here we're going to do the same
    unsigned remainder = frac & 0x1ff; // 9 least significant bits we're about to lose later
    unsigned mid_point = 0x100;

    if (remainder > mid_point || // we either round up because our number is closer to the upper number
        remainder == mid_point && (frac & 0x200)) { // or we're exactly ad mid-point and the 10th bit == 1
                                                  // (means we'll have an odd number after removing the extra 9 bits)
        if (frac + 0x200 < frac) { // in case rounding up will overflow the fraction
            exp++;
        }
        frac += 0x200; // round up
    }

    // * Important to note - we don't need to take care of the case of roudning down as this is already happens when we shift the fraction
    // and by this removing the 9 remainder bits
    
    int e = exp + bias;
    return sign | (e << 23) | (frac >> 9);
}

int main() {
    for (unsigned i = 0; i != UINT_MAX; i++) {
        int n = (int) i;
        float_bits fb = float_i2f(n);
        float f = *(float*) &fb;
        assert(f == (float) n);
        if (i % 42949672 == 0) printf("completed %.0f%%\n", (float) i / UINT_MAX * 100.0);
    }

    printf("done\n");

    return 0;
}
