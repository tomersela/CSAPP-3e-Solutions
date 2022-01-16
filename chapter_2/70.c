#include<assert.h>

/*
* Return 1 when x can be represented as an n-bit, 2’s-complement
* number; 0 otherwise
* Assume 1 <= n <= w
*/
int fits_bits(int x, int n) {
    // mask for bits after the nth bit
    unsigned mask = -1 << n;
    // in case there are bits after the nth bit, this number can't represent two's complement of length n
    return !(x & mask);
}

int main() {
    assert(fits_bits(0b00001010, 4));
    assert(!fits_bits(0b00001010, 3));
    return 0;
}