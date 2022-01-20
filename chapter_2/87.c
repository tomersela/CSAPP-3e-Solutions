/*
Description             Hex             M               E               V               D
-0                      7000            0               -14             -0              -0.0
Smallest value > 2      4001            1025/1024       1               1025/512        2.001953125
512                     2400            1               9               512             512.0
Largest denormalized    03FF            123/16777216    -14             1023/(2^24)     6.097555e-5
−∞                      FC00            -               -               −∞              −∞
Number with hex         3BB0            123/64          -1              123/128         0.9609375
 representation 3BB0


1 sign bit, 5 exponent bits (k = 5), and 10 fraction bits (n = 10)
-0:
    S = 1
    M = 0
    E = 0
    1 00000 0000000000
    0b1000 0000 0000 0000 = 0x7000

Smallest value > 2:
    let's first see the representation for 2:
    S = 0
    M = 1
    E = 1  =>  e - Bias = 1  =>  e - 15 = 1  =>  e = 16

    0 10000 0000000000

    so the next possible value after two can be calculated by adding 1 to the significand:
    0 10000 0000000001

    f = 0b0.0000000001 = 2^-10 = 1/1024
    M = 1 + f = 1 + 1/1024 = 1025/1024

    0b0100 0000 0000 0001 = 0x4001
    V = M*2^E = 1025/1024*2^1= 1025/512

512:
    512 = 2^9 = 0b1000000000
    f = 0
    M = 1 + f = 1
    E = 9
    S = 0

    0 01001 0000000000

    0b0010 0100 0000 0000 = 0x2400

Largest denormalized:
    0 00000 1111111111

    0b0000 0011 1111 1111 = 0x03FF
    e = 0
    E = 1 - Bias = -14
    f = 0b0.1111111111
    M = f = 0b0.1111111111 = 0b1111111111.0 * 2^-10 = 1023/(2^10)
    V = M*2^E = 1023/(2^10)*2^-14 = 1023*2^-24 = 1023/(2^24)

−∞:
    1 11111 0000000000

    0b1111 1100 0000 0000 = 0xFC00
    E = 2^5 - 1 = 31
    f = 0

Number with hex representation 3BB0:
    0x3BB0 = 0b0011 1011 1011 0000
    0 01110 1110110000
    f = 0b0.1110110000
    M = 1 + f = 0b1.1110110000 = 0b1111011.0000 * 2^-6 = 123/64
    e = 0b01110 = 14
    E = e - Bias = 14-15 = -1
    V = M*2^E = 0b1.1110110000 * 2^-1 = 0b0.11110110000 = 0b1111011.0000*2^-7 = 123*2^-7 = 123/128
    
*/

int main() {
    return 0;
}