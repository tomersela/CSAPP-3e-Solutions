/*
A. 0x40490FDB = 0b0100 0000 0100 1001 0000 1111 1101 1011

0 10000000 10010010000111111011011
S = 0
e = 0b10000000 = 128
E = e - Bias = 128 - 127 = 1
f = 0b0.10010010000111111011011
M = 0b1.10010010000111111011011
V = (-1)^S*M*2^E = 1 * 0b1.10010010000111111011011 * 2^1 = 0b11.0010010000111111011011

B. 22/7 = 3 + 1/7 = 3 + 1/(2^3-1) = 0b11.0 + 0b001/(2^3-1) = 0b11.0 + 0.001001001... = 0b11.001001001...
                                                           ^ see problem 83

C. 22/7       = 0b11.001001001001...
   0x40490FDB = 0b11.0010010000111111011011
                             ^ position 9 after the binary point
*/

int main() {
    return 0;
}