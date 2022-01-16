#include<stdio.h>

typedef char *byte_pointer;

void show_bytes(byte_pointer start, int len) {
    for (int i = 0; i < len; i++)
        printf("%.2hhx ", start[i]);
    printf("\n");
}

void show_int(unsigned int x) {
    show_bytes((byte_pointer) &x, sizeof(unsigned int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void test_show_bytes(int val) {
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}

int main() {
    test_show_bytes(123);
    return 0;
}