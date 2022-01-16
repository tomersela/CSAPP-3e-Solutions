#include<assert.h>

const int WORD_SIZE = sizeof(int) << 3;

unsigned srl(unsigned x, int k) {
    /* Perform shift arithmetically */
    unsigned xsra = (int) x >> k;
    
    // set a mask of 1's from position 0 to k'th bit
    unsigned mask = ~(-1 << (WORD_SIZE - k));
    return xsra & mask;
}

int sra(int x, int k) {
    /* Perform shift logically */
    int xsrl = (unsigned) x >> k;
    
    // get most significant bit of x
    int msb_mask = 0x1 << (WORD_SIZE - 1);
    int is_msb_eq_0 = !(x & msb_mask);

    // create an integer with all bits equal to the msb of x (either all 0's or all 1's)
    int msb_for_all_bits = (-1 + is_msb_eq_0);

    // k most significant bits as msb of x
    int mask = msb_for_all_bits << (WORD_SIZE - k);

    return xsrl | mask;
}

int main() {
    assert(srl(0xFEFFFFFF, 8) == 0x00FEFFFF);
    assert(srl(0x007C0000, 8) == 0x00007C00);
    assert(sra(0x8C000000, 8) == 0xFF8C0000);
    assert(sra(0x7C000000, 8) == 0x007C0000);
    return 0;
}