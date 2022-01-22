/*
Format A                                                                Format B
Bits                                Value                               Bits                                Value
1 01111 001                         -9/8                                1 0111 0010                         -9/8
0 10110 011                         176                                 0 1011 0110                         176
1 00111 010                         5/64                                1 0011 0100                         5/64
0 00000 111                         7/(2^17)                            0 0000 0001                         1/2^10
1 11100 000                         -(2^13)                             1 1110 1111                         -248
0 10111 100                         384                                 0 1111 0000                         +∞


1 01111 001
E = 0b01111-Bias = 15-15 = 0
f=-0b0.001
M=0b1.001 = 0b1001*2^-3 = 9/8
V=-2^0*9/8 = -9/8


0 10110 011
Format A
e = 0b10110 = 22
E = e - Bias = 22 - 15 = 7
M = 0b1.011 = 0b1011*2^-3 = 11*2^-3
8/8+2/8+1/8
V=2^7*11*2^-3 = 11*2^4 = 176

Format B
176 = 0b10110000 = 0b1011 * 2^4
E = 4 = e - Bias = e - 7  =>  e = 4 + 7 = 11 = 0b1011
0 1011 0110
1011*2^-3


1 00111 010
Format A
E = 7 - 15 = -8
M = 1 + 0b0.010 = 0b1.010 = 0b101.0 * 2^2 = 5 * 4 = 20
V = -20*2^-8 = -20/256 = -5/64

Format B
5/64 = 5*2^-6
V = -1*5*2^-6 = -1 * 0b101 * 2^-6 = -1 * 0b1.01 * 2^2 * 2^-6 = -1 * 0b1.01 * 2^-4 = -1 * (1 + 0b0.01) * 2^-4
M = 1 + 0b0.01  =>  f=0b0.01
E = -4 = e - Bias = e - 7  =>  e = -4 + 7 = 3
M = 5 = 0b101 = 0b1.01*2^-2  =>  f = 0b0.01
1 0011 0100


0 00000 111
Format A
E = 1 - e = 1 - 15 = -14
M = f=0b0.111 = 0b111*2^-3 = 7*2^-3
V = 7*2^-3*2^-14 = 7*2^-17 = 7/(2^17)

Format B
This number can't be represented accurately with format B.
The smallest positive number that can be represented is too big:
0 0000 0001
E = 1 - e = 1 - 7 = -6
M = f = 0b0.0001 = 0b1.0*2^-4
V = M*2^E = 2^-4*2^-6 = 2^-10

the original number from format A is closer to 0 than to 2^-10 but the problem states we should round toward +∞
therefore, we'll use the smallest positive number of format B


1 11100 000
Format A:
e = 0b11100 = 28
E = e - Bias = 28 - 15 = 13
f = 0.0
M = 1 + f = 1
V = (-1)^S*M*2^E =(-1)^1*1*2^13 = -(2^13)

Format B:
S = 1
M = 1
E = 13
e = E + Bias = 13 + 7 = 20  ->  bigger than our biggest e!
we can't represent this number, as our largest negative number isn't large enough.
we'd round down to -∞ unless it wasn't specified we should round up toward +∞, so we'll use the largest negative number
1 1110 1111
e = 14
E = e - Bias = 14 - 7 = 7
f = 0b0.1111
M = 0b1.1111 = 0b11111*2^-4 = 31*2^-4
V = -1 * 31*2^-4 * 2^7 = -31 * 2^3 = -31 * 8 = -248


0 10111 100
Format A:
e = 0b10111 = 23
E = e - Bias = 23 - 15 = 8
f = 0b0.1
M = 1 + f = 0b1.1 = 0b11*2^-1 = 3/2
V = 3/2 * 2^8 = 3 * 2^7 = 384

Format B:
S = 0
M = 0b11 = 0b11 * 2^-1  =>  f = 0b0.1
Now we should have E = 8 in order to move M's decimal point so it will be equal to 3 * 2^7
E = 8 = e - Bias = e - 7  =>  e = 15 Invalid value (our biggest E is twice the bias - 14)
Since we can't represent this value, we'll round up to +∞
0 1111 0000
*/

int main() {
    return 0;
}