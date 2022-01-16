#include<assert.h>

/*
A. the implementation in the question doesn't take into account the two's complement representation of the number extracted.
It considers all numbers to be unsigned.
*/

/* Declaration of data type where 4 bytes are packed
   into an unsigned */
typedef unsigned packed_t;

/* Extract byte from word.  Return as signed integer */
int xbyte(packed_t word, int bytenum) {
    int max_bytenum = 3;
    // shift the requested byte all the way to the msb position
    unsigned shifted_left = word << ((max_bytenum - bytenum) << 3);
    // shift the msb right to lsb using arithmetic right shift to preserve sign
    int with_sign = (int) shifted_left >> (max_bytenum << 3);
    return with_sign;
}

int main() {
    assert(xbyte(0x12F45678, 2) == -12);
    assert(xbyte(0x12F45678, 3) == 18);
    return 0;
}