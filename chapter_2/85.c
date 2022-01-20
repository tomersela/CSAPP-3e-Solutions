// V = (-1)^S * M * 2^E
// Bias = 2^(k-1) - 1
//
// Denormalized (e = 0b00..0, f != 0b00..0):
// E = 1 - Bias = 1 - (2^(k-1) - 1) = 1 - 2^(k-1) + 1
// M = f/(2^n)
// V = (-1)^S * M * 2^E
//   
// 
// Normalized (e != 0b00..0 and e != 0b11..1):
// E = e - Bias = e - (2^(k-1) - 1) = e - 2^(k-1) + 1
// M = 1 + f/(2^n)
// V = (-1)^S * M * 2^E
//   = (-1)^S * f/(2^n) * 2^e - 2^(k-1) + 1)
//   = (-1)^S * f * 2^(e - 2^(k-1) + 1 - n)

// A. 7
// 7 = (-1)^0 * f - 2^(k-1) + 1 - n)
// S = 0 
// M = 1 + f
// f = 0.111
// E = 2 = e - Bias  =>  e = 2 + Bias = 2 + 2^(k-1) - 1 = 1 + 2^(k+1)
// 2^(k-1) has the binary form 0..010..0 when 1 is added to position k-1 (msb), and 2 has the form 0b00..01
// Assuming k > 4, E = 10..011
// Binary: 0 10..01 1100..


// B. The largest odd integer that can be represented exactly
// S = 0
// M = 11..11 (b bits)
// E = n + bias (if we move more than n bits the number won't be odd)
// Binary: 0 (n + bias) 11..11


// C. The reciprocal of the smallest positive normalized value
// First, finding the smallest positive normalized value:
//     S = 0
//     E = 00..01 = e - Bias  => e = 1 + Bias
//     M = 1
//     f = 0.000..
//     V = (-1)^S*M*2^E = 1*1*2^(1-Bias) = 2^(1-Bias)
// The reciprocal of V:
// 1/V = 1/(2^(1-Bias)) = 2^(Bias-1)
//     = (-1)^0*1*2^(Bias-1)
//
//     S = 0
//     M = 1
//     E = Bias-1
//     but also E = e - Bias , therefore: Bias-1 = e - Bias
//     =>  e = 2*Bias - 1
//     since Bias = 2^(k-1) - 1:
//     e = 2*(2^(k-1) - 1) - 1 = 2^k - 3 = 0b11..101 (k bits)
// Binary: 0 11..101 00..00

int main() {
    return 0;
}