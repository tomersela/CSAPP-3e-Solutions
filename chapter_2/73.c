#include<limits.h>
#include<assert.h>

/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y) {
    int word_size = sizeof(int) << 3;
    int sign_shift = word_size - 1;

    int sum = x + y;
    unsigned x_sign = (unsigned) x >> sign_shift;
    unsigned y_sign = (unsigned) y >> sign_shift;
    int sum_sign = sum >> sign_shift;
    
    // there's an overflow if the signs of x and y are the same but the sign of sum is different
    int is_overflow = (x_sign == y_sign) & (x_sign ^ sum_sign);

    // if is_overflow is false then mask is all ones (-1 = 0b11..11), otherwise mask is 0
    unsigned mask = is_overflow - 1;
    
    int saturated = ~(INT_MIN - x_sign);
    
    // return saturated result in case of overflow, otherwize return sum
    return (mask & sum) | (~mask & saturated);
}

int main() {
    assert(saturating_add(50, 50) == 100);
    assert(saturating_add(-50, 50) == 0);
    assert(saturating_add(-50, -50) == -100);
    assert(saturating_add(INT_MAX, 1) == INT_MAX);
    assert(saturating_add(INT_MAX, INT_MAX) == INT_MAX);
    assert(saturating_add(INT_MIN, -1) == INT_MIN);
    assert(saturating_add(INT_MIN, INT_MIN) == INT_MIN);
    return 0;
}