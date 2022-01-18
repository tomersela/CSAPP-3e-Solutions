#include<limits.h>
#include<assert.h>

int mul3div4(int x) {
    int word_size = sizeof(int) << 3;
    int mul3 = (x << 1) + x;

    // x in case is negative we need to add a bias of 3 (2^2 - 1) - see 78.c for further explenation
    int biased = mul3 + ((mul3 >> (word_size - 1)) & 3);
    int mul3div4 = biased >> 2;
    return mul3div4;
}

int main() {
    assert(mul3div4(-2) == (int) -2 * 3 / 4);
    assert(mul3div4(INT_MAX) == INT_MAX * 3 / 4);
    assert(mul3div4(INT_MIN) == INT_MIN * 3 / 4);
    
    return 0;
}