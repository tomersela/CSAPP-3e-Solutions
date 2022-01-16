#include<assert.h>

/*
* Mask with least signficant n bits set to 1
* Examples: n = 6 --> 0x3F, n = 17 --> 0x1FFFF
* Assume 1 <= n <= w
*/
int lower_one_mask(int n) {
    int word_size = sizeof(int) << 3;
    return (unsigned) -1 >> (word_size - n);
}

int main() {
    assert(lower_one_mask(6) == 0x3F);
    assert(lower_one_mask(17) == 0x1FFFF);
    return 0;
}