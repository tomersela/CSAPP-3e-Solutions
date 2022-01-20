#include<assert.h>

int A(int k) {
    return -1 << k;
}

int B(int k, int j) {
    return ~(-1 << k) << j;
}

int main() {
    assert(A(8) == 0xFFFFFF00);
    assert(A(24) == 0xFF000000);
    assert(A(24) == 0xFF000000);
    assert(B(16, 8) == 0x00FFFF00);
    assert(B(8, 16) == 0x00FF0000);
    return 0;
}