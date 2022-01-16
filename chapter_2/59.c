#include<stdio.h>

int main() {
    int x = 0x89ABCDEF;
    unsigned int y = 0x76543210;

    int exp = (x & 0xFF) | (y & ~0xFF);

    printf("%04x", exp);
    return 0;
}