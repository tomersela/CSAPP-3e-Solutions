#include<assert.h>

// A. (float) x == (float) dx
//    Always 1
//    both float and double can represent bigger numbers than the largest integer, so we shouldn't worry about conversion to ∞ for x or y.
//    double precision is higher than float's precision, when dx is casted to float it's rounded if needed (x is rounded the same way).

void B() {
    int x = (1 << 31) - 1; // largest integer value
    int y = -1;

    double dx = (double) x;
    double dy = (double) y;

    // x - y = Tmax + 1  this will overflow to Tmin
    // while dx - dy will not overflow
    assert(dx - dy != (double) (x-y));
}

// C. (dx+dy)+dz==dx+(dy+dz)
//    Always 1:
//    In order to prove it, we need to show two things:
//    - dx, dy and dz can't have values in the range that makes double precision variables addition not associative
//        floating point (or double) variables are not associative in cases where two of the variables adds to +Infinity (or -Infinity),
//        while if we added other 2 variables first, we would get a result that not yield Infinity.
//        for example when dx=1, dy = +Inifinity and dz = -Inifinty:
//        (dx+dy)+dz = (1 + Infinity) + -Infinity = Infinity + -Infinity = 0  while:
//        dx+(dy+dz) = 1 + (Infinity + -Infinity) = 1 + 0 = 1
//    - any addition of 2 integers (as doubles) will not be rounded
//      if for example, the other claim was true, we could set a dx, dy, dz to numbers such as:
//      (dx+dy)+dz = rounded_number + dz  // while rounded_number != dx+dy in practice
//      and:
//      dx+(dy+dz) = dx + not_rounded = good_result
//      in order to prove that double can represent any sum of int, let's check the number of bits required to represent Tmax+Tmax
//      the result should have the longest bits required by a sum of 2 integers.
//      Tmax+Tmax = 2^31 - 1 + 2^31 - 1 = 2^32 - 2 = 0b11111111111111111111111111111110
//      we'll need 31 bits in the significant to represent that, we have 52 so we're good.
//      also the exponent has a very wide range which allows us to easily shift left these bits (we only need an exponent of 31 or less).
//      so the sum of any 2 doubles in the range of integer variables will not be rounded

// D. (dx*dy)*dz==dx*(dy*dz)
void D() {
    int x = 0x10000001;
    int y = 0x70000007;
    int z = 0x1000000C;


    double dx = (double) x;
    double dy = (double) y;
    double dz = (double) z;

    unsigned long lx = (unsigned long) x;
    unsigned long ly = (unsigned long) y;
    unsigned long lz = (unsigned long) z;

    // (lx*ly) = 504403162023591943
    // (dx*dy) = 504403162023591936.0
    // (ly*lz) = 504403182693122132
    // (ly*dz) = 504403182693122112.0
    // (dx*dy)*dz = 504403162023591936.0 * 1879048199.0 = 135399698858482734823636992.0
    // dx*(dy*dz) = 268435457.0 * 504403182693122112.0  = 135399698858482717643767808.0

    assert((dx*dy)*dz!=dx*(dy*dz));
}

void E() {
    // Not always 1
    int x = 0;
    int z = 1;
    double dx = (double) x;
    double dz = (double) z;

    assert(dx/dx != dz/dz); // dx/dx = 0/0 = NaN
}

int main() {
    B();
    D();
    E();
    return 0;
}