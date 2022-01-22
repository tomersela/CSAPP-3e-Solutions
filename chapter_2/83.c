// A. let x be a number of the form 0.yyyyyyyyyyyyy..., while yy..y is an infinite sequence of repeated k bits number.
//    and let Y = B2Uk(y) that is, the number having binary representation yy..y (k bits)
//        x << k = yy..y.yyyy... = Y + x
//    since x << k = x*2^k:
//        => x * 2^k = Y + x
//        => x * 2^k - x = Y
//        => x * (2^k - 1) = Y
//        => x = Y/(2^k - 1)


// B.
//     (a) 101 -> 3/(2^4-1) = 3/15 = 1/5
//     (b) 0110 -> 6/(2^4-1) = 6/15 = 2/5
//     (c) 010011 -> 19/(2^6-1) = 19/63


int main() {
    return 0;
}