#include<assert.h>

int int_shifts_are_arithmetic() {
    // ob11...11
    int all_ones = -1;
    int shifted = all_ones >> 1;
    return !~(shifted & all_ones);
}

int main() {
    assert(int_shifts_are_arithmetic());
    return 0;
}