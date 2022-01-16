#include<stdio.h>
#include<assert.h>

const int UNSIGNED_SIZE = (int) sizeof(unsigned);

unsigned replace_byte(unsigned x, int i, unsigned char b) {
    assert(i > 0);
    assert(i < UNSIGNED_SIZE);
    int bits_shift = i << 3;
    unsigned with_i_byte_masked = x & ~(0xFF << bits_shift);
    return with_i_byte_masked | b << bits_shift;
}


int main() {
    printf("%04x\n", replace_byte(0x12345678, 2, 0xAB));
    printf("%04x\n", replace_byte(0x12345678, 0, 0xAB));
    return 0;
}