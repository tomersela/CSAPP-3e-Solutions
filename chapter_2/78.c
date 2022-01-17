#include<assert.h>
#include <math.h>

/* Divide by power of 2. Assume 0 <= k < w-1 */
int divide_power2(int x, int k) {
    int word_size = sizeof(int) << 3;

    // `/` rounds towards zero, so we need to have the same behavior
    // unfortunately `>>` rounds down instead of towards zero: x>>k is equivalent to floor(x/(2^k)) in real numbers
    // in case x is positive, rounding down is good since it going towards zero
    // but when x is negative, and the number isn't exactly a multiplication of 2^k, then it will round down and we need to add 1 to the shift result
    
    // if x is a multiplication of 2^k it means there's no remainder, and all k least significant bits are all zeros
    // but if there's a remainder, there's at least one 1 in k least significant bits
    unsigned is_remainder = (unsigned) x << (word_size - k);
    unsigned is_negative = (unsigned) (x & (1 << (word_size - 1)));

    return ((int)(x >> k)) + (is_remainder && is_negative);
}

int divide_power2_opt2(int x, int k) {
    // this option is based on the simple idea that if shift right works well for positive numbers, let's use it on positive numbers only.
    // in case x is negative, we'll convert it to a postive number (-x), do the shift, then convert it to a negative number again
    int word_size = sizeof(int) << 3;
    int negative_mask = x >> (word_size - 1); // all ones in case x is negative, all zeros otherwise
    return (negative_mask & -(-x >> k)) | (~negative_mask & (x >> k));
}

int divide_power2_opt3(int x, int k) {
    /*
    here we'll acheive the same effect of rounding up negative numbers, but instead of adding 1 to the shift result,
    we'll add 2^k - 1 to x before shifting. the following shows why it has the same effect:
    
    Let's first see what happen when we don't add the bias value:
    - if x is a multiplication of 2^k (x = -z * 2^k):
        x >> k = floor(x/(2^k)) = floor(-z*(2^k)/(2^k)) = floor(-z) = -z
    - if x is a multiplication of 2^k and some remainder (x = -z * 2^k - r)
        x >> k = floor(x/(2^k)) = floor((-z*(2^k) - r)/(2^k)) = floor(-z - r/(2^k)) = ...
            since r/(2^k) > 0:
        ... = floor(-z - r/(2^k)) = -z - 1
        in this case we rounded down which isn't what we wanted
    
    Now let's see what happens if we add a bias value of 2^k - 1 to x before the shift:
    u = x + 2^k - 1
    u >> k = (x + 2^k - 1) >> k = floor((x + 2^k - 1)/(2^k))
    - if x is a multiplication of 2^k (x = -z * 2^k):
        u >> k = floor((x + 2^k - 1)/(2^k)) = floor((-z * 2^k + 2^k - 1)/(2^k)) = floor(-z + 1 - 1/(2^k))
            since 1 - 1/(2^k) < 1:
        u >> k = floor(-z + 1 - 1/(2^k)) = -z
        nothing changed so far, good.
    - if x is a multiplication of 2^k and some remainder (x = -z * 2^k - r)
        u >> k = floor((x + 2^k - 1)/(2^k)) = floor((-z * 2^k - r + 2^k - 1)/(2^k)) = floor(-z - r/(2^k) + 1 - 1/(2/k))
        = floor(-z + 1 - (r+1)/(2^k))
            r < 2^k (otherwize, r couldn't be a remainder of diviation by 2^k), therefore, r + 1 <= 2^k and hence (r+1)/(2^k) <= 1.
            this yields 1 >= 1 - (r+1)/(2^k) >= 0, so now:
      floor(-z + 1 - (r+1)/(2^k)) = -z
      u >> k = -z
    
    so by adding a bias value of 2^k-1 to a negative x gives us the same effect
    */

    int word_size = sizeof(int) << 3;

    int is_negative = (unsigned) (x & (1 << (word_size - 1))) >> (word_size - 1);

    // if is_negative = 0 then rounded = x
    // if is_negative = 1 then rounded = x + (1 << k) - 1;
    int rounded = x + (is_negative << k) - is_negative;
    return rounded >> k;
}
int divide_power2_opt4(int x, int k) {
    // Taken from https://stackoverflow.com/a/5160478/1163424 (modified to support any word size)
    // This is based on the same idea as divide_power2_opt3, but nicer on the bit arithmetic magic
    // (1 << k) + ~0 is equivalent to 2^k - 1
    // x >> (word_size - 1) yields all ones in case of negative x and all zeros in case of positive x
    // therefore, ((x >> (word_size - 1)) & ((1 << k) + ~0) will be equal to 2^k -1 when x is negative and 0 when x is positive
    int word_size = sizeof(int) << 3;
    return (x + ((x >> (word_size - 1)) & ((1 << k) + ~0))) >> k;
}

int main() {
    int x = 0xF0000011;
    int test_tuples[7][2] = {
        {1, 1},
        {15, 3},
        {-7, 1},
        {-8, 1},
        {x, 1},
        {x, 2},
        {x, 3},
    };

    for (int i = 0; i < 7; i ++) {
        int *tuple = test_tuples[i];
        int x = tuple[0];
        int k = tuple[1];
        assert(divide_power2(x, k) == x / ((int) pow(2, (double) k)));
        assert(divide_power2_opt2(x, k) == x / ((int) pow(2, (double) k)));
        assert(divide_power2_opt3(x, k) == x / ((int) pow(2, (double) k)));
        assert(divide_power2_opt4(x, k) == x / ((int) pow(2, (double) k)));
    }
    
    return 0;
}