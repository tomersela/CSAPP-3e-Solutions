#include<stdio.h>

typedef char *byte_pointer;

int is_little_endian() {
    int i = 1;
    byte_pointer b = (byte_pointer) &i;
    return (int) *b;
}

int main() {
    printf("is_little_endian: %d\n", is_little_endian());
    return 0;
}