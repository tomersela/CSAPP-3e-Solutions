#include<assert.h>

int mul_17(int x) {
    // x * 17 = x * (16 + 1) = x * 2^4 + x
    return (x << 4) + x;
}


int mul_minus_7(int x) {
    // x * -7 = x * (1 - 8) = x - x * 2^4
    return x - (x << 3);
}


int mul_60(int x) {
    // x * 60 = x * (64 - 4) = x * 2^6 - x * 2^2
    return (x << 6) - (x << 2);
}


int mul_minus_112(int x) {
    // x * -112 = x * (16 - 128) = x * 2^4 - x * 2^7
    return (x << 4) - (x << 7);
}

int main() {
    assert(mul_17(3) == 3 * 17);
    assert(mul_minus_7(3) == 3 * -7);
    assert(mul_60(3) == 3 * 60);
    assert(mul_minus_112(3) == 3 * -112);
    return 0;
}