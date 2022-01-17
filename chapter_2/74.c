#include<limits.h>
#include<assert.h>
#include<stdio.h>

/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y) {
    int word_size = sizeof(int) << 3;
    int sign_shift = word_size - 1;
    int diff = x - y;

    int x_sign = x >> sign_shift;
    int y_sign = y >> sign_shift;
    int diff_sign = diff >> sign_shift;
    // if x is positive and y is negative and diff is negative -> overflow
    int overflow = x_sign & !y_sign & !diff_sign;
    // if x is negative and y is positive and diff is positive -> underflow
    int underflow = (!x_sign) & y_sign & diff_sign;

    return !(overflow | underflow);
}

int main() {
    assert(!tsub_ok(INT_MIN, 1));
    assert(!tsub_ok(INT_MAX, -1));
    assert(tsub_ok(-5, -5));
    assert(tsub_ok(5, -5));
    assert(tsub_ok(-5, 5));
    assert(tsub_ok(5, 5));
    return 0;
}