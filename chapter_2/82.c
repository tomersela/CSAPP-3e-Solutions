#include<limits.h>
#include<assert.h>

void A() {
    // Not true when x = Tmin
    int x = INT_MIN;
    int y = 1;
    assert((x<y) != (-x>-y));
}

// B. ((x+y)<<4) + y-x == 17*y+15*x
// Always 1
// (x+y) << 4 + y - x = (x + y)*2^4 + y - x = 16*x + 16*y + y - x = 15*x + 17*y

// C. ~x+~y+1==~(x+y)
// Always 1
// We'll use this identity of negating a number in two's complement representation:
//     -a = ~a + 1
// ~x+~y+1 = ~x + 1 - 1 + ~y + 1 = -x - 1 + ~y + 1 = -x - 1 - y = -(x + y) - 1 = ~(x + y)


// D. (ux-uy) == -(unsigned)(y-x)
// Always 1
// let xmsb and ymsb be the most significant bits of x and y accordingly.
// (1)  (ux-uy) = T2U(x)-T2U(y)
// Now, according to Equation 2.6 (T2U(x) = x + xmsb*2^32):
// (2)  T2U(x)-T2U(y) = x+xmsb*2^32-(y+ymsb*2^32) = x-y + xmsb*2^32-ymsb*2^32 = x-y + (xmsb-ymsb)*2^32 = -(y-x) - (ymsb-xmsb)*2^32
//      = -((y-x) + (ymsb-xmsb)*2^32)
// using Equation 2.6 again:
// (y-x) + (ymsb-xmsb)*2^32 = T2U(y-x)
// therefore:
// (3)  -((y-x) + (ymsb-xmsb)*2^32) = -T2U(y-x)
// by chainin (1), (2), and (3), we concolude that (ux-uy) = -T2U(y-x)  or in c:
// (ux-uy) == -(unsigned)(y-x)

// E. ((x>>2)<<2)<=x
// Always 1
// ((x>>2)<<2) converts the 2 least significant bits to 0, causing a reduction in the range of 0 to 3 from x.

int main() {
    A();
    return 0;
}