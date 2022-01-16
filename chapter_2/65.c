#include<assert.h>

/* Return 1 when x contains an odd number of 1s; 0 otherwise.
   Assume w=32 */
int odd_ones(unsigned x) {
    unsigned temp = x;
    temp ^= temp >> 16;
    temp ^= temp >> 8;
    temp ^= temp >> 4;
    temp ^= temp >> 2;
    temp ^= temp >> 1;
    return temp &= 1;
}

int main() {
    assert(odd_ones(0xFE));
    assert(!odd_ones(0xFF));
    return 0;
}