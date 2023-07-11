#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

typedef void (*psumFunc)(float[], float[], long);

/* Compute prefix sum of vector a */
void psum1(float a[], float p[], long n)
{
    long i;
    p[0] = a[0];
    for (i = 1; i < n; i++)
        p[i] = p[i-1] + a[i];
}

void psum2(float a[], float p[], long n)
{
    long i;
    p[0] = a[0];
    for (i = 1; i < n-1; i+=2) {
        float mid_val = p[i-1] + a[i];
        p[i] = mid_val;
        p[i+1] = mid_val + a[i+1];
    }

    /* For even n, finish remaining element */
    if (i < n)
        p[i] = p[i-1] + a[i];
}

/* Book's solution for Problem 5.12 */
void psum1a(float a[], float p[], long n)
{
    long i;
    float last_val, val;
    last_val = p[0] = a[0];
    for (i = 1; i < n; i++) {
        val = last_val + a[i];
        p[i] = val;
        last_val = val;
    }
}

/* My solution for Problem 5.12 */
void psum1b(float a[], float p[], long n)
{
    long i;
    p[0] = a[0];
    for (i = n - 1; i > 0; i--)
        p[i] = p[i-1] + a[i];
}

/* unrolled */
void psum4a(float a[], float p[], long n)
{
    long i;
    float last_val, val;
    float val0, val1, val2, val3, val0_1, val2_3;
    float last_0_1;
    last_val = p[0] = a[0];
    for (i = 1; i < n - 4; i += 4) {
        val0 = a[i];
        val1 = a[i+1];
        val2 = a[i+2];
        val3 = a[i+3];

        // p[i] = last_val + val0;
        // p[i+1] = last_val + val0 + val1;
        // p[i+2] = last_val + val0 + val1 + val2;
        // val = last_val + val0 + val1 + val2 + val3;
        // p[i+3] = val;
        // last_val = val;
        val0_1 = val0 + val1;
        val2_3 = val2 + val3;
        last_0_1 = last_val + val0_1;
        p[i] = last_val + val0;
        p[i+1] = last_0_1;
        p[i+2] = last_0_1 + val2;
        val = last_0_1 + val2_3;
        p[i+3] = val;
        last_val = last_val;
    }

    for (; i < n; i++) {
        val = last_val + a[i];
        p[i] = val;
        last_val = val;
    }
}

void test_psum(char* name, psumFunc func) {
    unsigned long size = 100000000;
    float* a = (float*) malloc(size * sizeof(float));
    float* p = (float*) calloc(sizeof(float), size);

    for (long i; i < size; i++) {
        p[i] = (double) (i % 20);
    }
    
    
    clock_t start_time = clock();
    func(a, p, size - 2);
    clock_t total = (clock() - start_time);
    
    free(a);
    free(p);

    
    printf("[%s]: \t\t%ld ticks\n", name, total);;
}

int main() {
    test_psum("psum1", psum1);
    test_psum("psum2", psum2);
    test_psum("psum1a", psum1a);
    test_psum("psum1b", psum1b);
    test_psum("psum4a", psum4a);

    /*
    Output on my M1 Pro machine:
     [psum1]: 		516855 ticks
     [psum2]: 		510733 ticks
     [psum1a]: 		695623 ticks
     [psum1b]: 		310836 ticks
     [psum4a]: 		305113 ticks
    */

    return 0;
}
