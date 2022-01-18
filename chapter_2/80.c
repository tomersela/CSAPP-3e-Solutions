#include<limits.h>
#include<assert.h>
#include<stdio.h>

// borrowed from 79.c
int mul3div4(int x) {
    int word_size = sizeof(int) << 3;
    int mul3 = (x << 1) + x;

    int biased = mul3 + ((mul3 >> (word_size - 1)) & 3);
    int mul3div4 = biased >> 2;
    return mul3div4;
}

int threefourths(int x) {
    // we'll slpit x to two halfs:
    // x = 2^16*msb + lsb
    // 3/4*x = 3/4*(2^16*a + b) = 3/4*a*2^16 + 3/4*b = 3*a*2^14 + 3/4*b
    // now, we can compute 3*a and 3/4*b without worries about overflow when multiplying by 3,
    // then shift left 3*a result by 14, and add the result of 3/4*b to it.
    //
    // * important to note that if x is negative, we should extend b with 1's to the left so it will be negative
    // * also important - we'll need to add a bias value of 3 to 3*a in case of a negative number (since dividing by shift is rounding down)
    //
    // Note: there's a reason why I didn't reuse the same 3/4*x operation for both a and b.
    // calculation 3/4*a for a will make use lose precision when we divide by 4. these 2 lsb's are very significant once we shift left by 16

    int word_size = sizeof(int) << 3;
    int half_word_size = word_size >> 1;
    int sign_mask = x >> (word_size - 1); // all ones in case negative, all zeros otherwise
    int msb = x >> half_word_size;
    int lsb_mask = ~(-1 << half_word_size);
    int lsb = x & lsb_mask | (sign_mask << half_word_size);
    int bias = sign_mask & 3;
    int msb_mul3 = (msb << 1) + msb + bias;
    
    return (msb_mul3 << (half_word_size-2)) + mul3div4(lsb);
}


// Nicer solution I saw at https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter2/2.80/
// I modified the code a bit to avoid using `&&` as a condition statement (it's an `if` in a mask if you ask me,
// and `if` is against the "Bit-Level Integer Coding Rules" mentioned in the book)
int threefourths_opt2(int x) {
    int word_size = sizeof(int) << 3;
    int sign_flag = x >> (word_size - 1); // all ones in case x is negative, all zeros otherwise
    int l = x & 0x3; // save the 2 least significant bits

    int fd4 = x >> 2; // divide x by 4 (losing 2 lsb)
    int fd4m3 = (fd4 << 1) + fd4; // then multiply by 3 (not taking into account the 2 least significant bits)

    int lm3 = (l << 1) + l; // multiply lsbs by 3
    int bias = sign_flag & ((1 << 2) - 1); // adding bias of (2^k-1) in case the number is negative
    int lm3d4 = (lm3 + bias) >> 2;

    return fd4m3 + lm3d4;
}

int main() {
    assert(threefourths(4) == 3);
    assert(threefourths(-4) == -3);
    assert(threefourths(-8) == -6);
    assert(threefourths(-10) == -7);
    assert(threefourths(INT_MAX) == 0x5FFFFFFF);
    assert(threefourths(INT_MIN) == 0xA0000000);

    assert(threefourths_opt2(4) == 3);
    assert(threefourths_opt2(-4) == -3);
    assert(threefourths_opt2(-8) == -6);
    assert(threefourths_opt2(-10) == -7);
    assert(threefourths_opt2(INT_MAX) == 0x5FFFFFFF);
    assert(threefourths_opt2(INT_MIN) == 0xA0000000);
    
    return 0;
}