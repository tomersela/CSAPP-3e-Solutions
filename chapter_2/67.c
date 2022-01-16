#include<assert.h>

/*
A. bad_int_size_is_32 fails on some machines since shift with number bigger then w
is undefined and can yield peculiar result (!= 0)
*/

int int_size_is_32() {
    /* Set most significant bit (msb) of 32-bit machine */
    int set_msb = 1 << 31;
    /* Shift past msb of 32-bit word */
    int beyond_msb = set_msb << 1;
    /* set_msb is nonzero when word size >= 32
    beyond_msb is zero when word size <= 32  */
    return set_msb && !beyond_msb;
}

int int_size_is_32_16_bit_proof() {
    int set_msb = 1 << 15 << 15 << 1;
    /* Shift past msb of 32-bit word */
    int beyond_msb = set_msb << 1;
    /* set_msb is nonzero when word size >= 32
    beyond_msb is zero when word size <= 32  */
    return set_msb && !beyond_msb;

}

int main() {
    assert(int_size_is_32());
    assert(int_size_is_32_16_bit_proof());
    return 0;
}