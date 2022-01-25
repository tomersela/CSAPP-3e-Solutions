#include<assert.h>
#include<float.h>
#include <math.h>

float u2f(unsigned x) {
    return *(float*) &x;
}

/*
V = M*2^E
M is 23 bits, therefore ranges between 2^-23 to 2^-1 for denormalized values
for normalized values, the highest number in the form of 2^x is 2^0 = 1
the exponent is 8 bits, and its value ranges between -127 to 128
  
  The lowest number we can represent in the shape of 2^x will be compounded by:
  - lowest M for denormalized values: 2^-23
  - lowest exponent value: 1 - bias = -126

  V = M*2^E = 2^-23*2^-126 = 2^-149

  The smallest normalized floating-point representation of 2x will be compounded by:
  - lowest (and only M) for normalized values of the shape 2^x: 1 (significant = 0)
  - lowest exponent value denormalized value:
    e = 0b00000001
    E = e - Bias = 1 - 127 = - 126
  V = 1*2^-126 = 2^-126
  we'll use that as the top boundary for denormalized values

  The highest normalized floating-point representation of 2x will be compounded by:
  - highest (and only M) for normalized values of the shape 2^x: 1 (significant = 0)
  - highest exponent value = 127
*/

const bias = 127;

float fpwr2(int x) {
    /* Result exponent and fraction */
    unsigned exp, frac;
    unsigned u;
    if (x < -149) {
        /* Too small. Return 0.0 */
        exp = 0;
        frac = 0;
    } else if (x < -126) { /* Denormalized result */
        exp = 0; // E = -127
        frac = 1 << (x + bias + 22); // frac ranges between 2^-23 to 2^-1
    } else if (x < 128) { /* Normalized result. */
        exp = x + bias;
        frac = 0; // M = 1 (implicitly)
    } else {
        /* Too big. Return +oo */
        exp = 0b11111111;
        frac = 0;
    }
    /* Pack exp and frac into 32 bits */
    u = exp << 23 | frac;
    /* Return as float */
    return u2f(u);
}

int main() {
    // Too small
    assert(fpwr2(-200) == 0);
    assert(fpwr2(-150) == 0);

    // Denormalized cases
    assert(fpwr2(-149) == pow(2, -149));
    assert(fpwr2(-127) == pow(2, -127));

    // Normalized cases
    assert(fpwr2(-126) == pow(2, -126));
    assert(fpwr2(-64) == pow(2, -64));
    assert(fpwr2(-4) == pow(2, -4));
    assert(fpwr2(-1) == pow(2, -1));
    assert(fpwr2(1) == pow(2, 1));
    assert(fpwr2(4) == pow(2, 4));
    assert(fpwr2(127) == pow(2, 127));

    // Too big
    assert(fpwr2(128) == INFINITY);
    assert(fpwr2(130) == INFINITY);


}