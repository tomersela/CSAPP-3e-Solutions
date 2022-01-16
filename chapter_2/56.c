#include<stdio.h>
#include<string.h>

typedef char *byte_pointer;

void show_bytes(byte_pointer start, int len) {
    for (int i = 0; i < len; i++)
        printf("%.2hhx ", start[i]);
    printf("\n");
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

int main() {
    int ival = 123;
    const char *s = "abcdef";
    show_bytes((byte_pointer) &ival, sizeof(int));
    show_bytes((byte_pointer) s, strlen(s));
    return 0;
}