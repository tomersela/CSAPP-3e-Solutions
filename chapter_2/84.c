#include<stdio.h>
#include<assert.h>
#include<float.h>

unsigned f2u(float x) {
    return *(unsigned*) &x;
}

int float_le(float x, float y) {
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);
    /* Get the sign bits */
    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;
    /* Give an expression using only ux, uy, sx, and sy */
    return (sx && !sy) // x <= 0 =< y
        || (!(sx | sy) && ux <= uy) // sx == sy == 0 (means x, y >= 0) and x <= y
        || ((sx & sy) && ux >= uy); // sx == sy == 1 (means x, y <= 0) and x <= y
}

int main() {
    assert(float_le(0.2, 0.7));
    assert(!float_le(0.7, 0.2));
    assert(float_le(-0, +0));
    assert(float_le(+0, -0));
    assert(float_le(0, 3));
    assert(float_le(-4, -0));
    assert(!float_le(-0, -4));
    assert(float_le(-4, 4));
    assert(float_le(-4.5, 4.5));
    assert(float_le(-4.5, -4.5));
    return 0;
}