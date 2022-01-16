#include<assert.h>

/*
 * Generate mask indicating leftmost 1 in x.  Assume w=32.
 * For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
 * If x = 0, then return 0.
 */
int leftmost_one(unsigned x) {
    // set all bits from 0 until leftmost 1 to 1
    unsigned temp = x | (x >> 16);
    temp |= temp >> 8;
    temp |= temp >> 4;
    temp |= temp >> 2;
    temp |= temp >> 1;
    // Now we have a number of the form 0..011..1, we shift it left and add one
    // to get a number of the form 0..010..0
    // in case x was zero we won't add 1
    return (temp >> 1) + (temp & 1);
}

int main() {
    assert(leftmost_one(0xFF00) == 0x8000);
    assert(leftmost_one(0x6600) == 0x4000);
    assert(leftmost_one(0x0000) == 0x0000);
    assert(leftmost_one(0x0001) == 0x0001);
    assert(leftmost_one(0x0003) == 0x0002);
    assert(leftmost_one(-1) == 0x1 << 31);
    return 0;
}