#include<stdio.h>
#include<string.h>

typedef char *byte_pointer;

void show_bytes(byte_pointer start, int len) {
    for (int i = 0; i < len; i++)
        printf("%.2hhx ", start[i]);
    printf("\n");
}

void show_short(short x) {
    show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long));
}

void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double));
}

int main() {
    short sval = 123;
    long lval = (long) sval;
    double dval = 1.5;
    show_short(sval);
    show_long(lval);
    show_double(dval);
    return 0;
}