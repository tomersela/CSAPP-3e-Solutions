#include<limits.h>
#include<assert.h>

int any_bit_eq_1(int x) {
    return !!x;
}

int any_bit_eq_0(int x) {
    return !!~x;
}

int any_lsb_bit_eq_1(int x) {
    // Change all bytes to 0x00 except LSB
    int masked = x & 0xFF;
    return any_bit_eq_1(masked);
}

int any_msb_bit_eq_0(int x) {
    // Create a mask with 0x00 as the most significant byte and 0xFF for the rest
    int msb_mask = ~(0xFF << ((sizeof(int) -1) << 3));
    // Turn all bits to one except MSB
    int masked = x | msb_mask;
    return any_bit_eq_0(masked);
}

int main() {
    assert(!any_bit_eq_1(0));
    assert(any_bit_eq_1(0x001000));
    
    assert(!any_bit_eq_0(-1));
    assert(any_bit_eq_0(-2));
    
    // create a ingeger with hex representation of 0xFF..FF00
    int all_ones_but_lsb = -1 & ~0xFF;
    assert(!any_lsb_bit_eq_1(all_ones_but_lsb));
    assert(any_lsb_bit_eq_1(all_ones_but_lsb + 1));

    // create a ingeger with hex representation of 0xFF00..00
    int all_zeros_but_msb = 0xFF << ((sizeof(int) - 1) << 3);
    assert(!any_msb_bit_eq_0(all_zeros_but_msb));

    // integer with 0xFE00..00 hex representation
    int msb_fe = 0xFE << ((sizeof(int) - 1) << 3);
    assert(any_msb_bit_eq_0(msb_fe));
    return 0;
}