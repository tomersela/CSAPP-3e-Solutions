#include <assert.h>

int signed_high_prod(int x, int y) {
    long long mul = ((long long) x) * ((long long) y);
    return mul >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y) {
    // if the msb of x or y is 1, signed_high_prod will treat it as negative number and will extended with ones when converted to 64 bit representation.
    // Let's assume the msb of x is 1, the 64 bit representation of it as signed number:
    // x = 0xFFFFFFFF[original x bits]
    // and the product signed_high_prod will return:
    // signed_prod = 0xFFFFFFFF[original x bits] * y = 0xFFFFFFFF00000000 * y + 0x00000000[original x bits] * y
    //
    // But we want to compute the high order bits of unsigned multiplication, and in the case of unsigned multiplication
    // x is extended to 64 bits with zeros:
    // x = 0x00000000[original x bits]
    // so in the unsigned multiplication x:
    // unsigned_prod = 0x00000000[original x bits] * y
    //
    // is we look at signed_prod as unsigned, there's a difference of (0xFFFFFFFF00000000 * y) from unsigned_prod:
    // ((unsigned) signed_prod) - unsigned_prod = 0xFFFFFFFF00000000 * y + 0x00000000[original x bits] * y - (0x00000000[original x bits] * y)
    // = 0xFFFFFFFF00000000 * y
    // and specifically the high order bits has a difference of 0xFFFFFFFF*y. so we can substract it from signed_prod result to get the high order bits
    // of the unsigned multiplication:
    // high_order_unsigned_prod = high_order_signed_prod - (0xFFFFFFFF * y)
    // 
    // since 0xFFFFFFFF is -1 in 32 bit two's complement: - (0xFFFFFFFF * y) = - (-1 * y) = y
    // therefore:
    // high_order_unsigned_prod = high_order_signed_prod - (0xFFFFFFFF * y) = high_order_signed_prod + y
    //
    // the same case can happen in case y is negative:
    // unsigned_mul = ((unsigned) high_order_signed_mul) + x

    int word_size = sizeof(unsigned) << 3;
    unsigned y_mask = (int) x >> (word_size - 1); // all ones in case x is negative or all zeros in case x is positive
    unsigned x_mask = (int) y >> (word_size - 1); // all ones in case y is negative or all zeros in case y is positive
    
    int signed_prod = signed_high_prod(x, y);
    return signed_prod + (y_mask & y) + (x_mask & x);;
}

unsigned unsigned_high_prod_for_test(unsigned x, unsigned y) {
    unsigned long long mul = (unsigned long long) x * y;
    return mul >> 32;
}

int main(int argc, char* argv[]) {
    unsigned x = -1;
    unsigned y = 0x12345678;

    assert(unsigned_high_prod(0xFFFFFFFF, 0x12345678) == unsigned_high_prod_for_test(0xFFFFFFFF, 0x12345678));
    assert(unsigned_high_prod(0xFFFFFFEA, 0x00000006) == unsigned_high_prod_for_test(0xFFFFFFEA, 0x00000006));
    return 0;
}