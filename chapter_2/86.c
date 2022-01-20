// sign bit, k = 15 exponent bits, a single integer bit, and n = 63 fraction bits
// Bias = 2^(k-1) - 1 = 2^14 - 1 = 16383

// Smallest positive denormalized
// 0 000000000000000 0 000000000000000000000000000000000000000000000000000000000000001
// E = 1 - Bias = -16382
// M = 0b0.000000000000000000000000000000000000000000000000000000000000001 = 1*2^-63 = 2^-63
// Decimal = M*2^E = 2^-63*2^(1-Bias) = 2^(1-Bias-63) = 2^(-Bias-63) = 2^(-16383-63) = 2^16446

// Smallest positive normalized
// 0 000000000000001 1 000000000000000000000000000000000000000000000000000000000000000
// E = e - Bias = 1 - Bias = -16382
// M = 1
// Decimal = M*2^E = 1*2^-16382 = 2^-16382

// Largest normalized
// 0 111111111111110 1 111111111111111111111111111111111111111111111111111111111111111
// e is the maximum value for normalized value which means it equals twice the Bias value ( e = 2^k - 2 = 2*(2^(k-1) -1) )
// E = e - Bias = 2*Bias - Bias = Bias
// M = 0b1.111111111111111111111111111111111111111111111111111111111111111 = 2 - 2^-63
// Decimal =  M*2^E = (2 - 2^-63) * 2^Bias = ~2*2^16383

int main() {
    return 0;
}