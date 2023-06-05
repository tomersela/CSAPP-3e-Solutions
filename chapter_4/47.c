#include<assert.h>

/* Bubble sort: Array version */
void bubble_a(long *data, long count) {
    long *curr;
    long *last;
    for (last = data + (count - 1); last >= data; last--) {
        for (curr = data; curr < last; curr++)
            if (*(curr + 1) < *curr) {
                /* Swap adjacent elements */
                long t = *(curr + 1);
                *(curr + 1) = *curr;
                *curr = t;
            }
    }
}

int main () {
    long arr[] = {3, 1, 2};
    long expected[] = {1, 2, 3};
    bubble_a(arr, 3);
    for (int i=0; i < 3; i++) {
        assert(arr[i] == expected[i]);
    }
}