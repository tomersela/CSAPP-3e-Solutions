#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

typedef double (*polyhFunc)(double[], double, long);

/* Apply Horner's method */
double polyh(double a[], double x, long degree)
{
    long i;
    double result = a[degree];

    for (i = degree - 1; i >= 0; i--) {
        result = a[i] + x * result;
    }

    return result;
}

double polyh_4x1a(double a[], double x, long degree)
{
    double x_2 = x * x;
    double x_3 = x_2 * x;
    double x_4 = x_3 * x;
    
    long i;
    double tmp0, tmp1, tmp2, tmp3, tmp_total;
    double result = a[degree];
    for (i = degree - 1; i >= 3; i -=4) {
        // result = a[i-3] + x * (a[i-2] + x * (a[i-1] + x * (a[i] + x * result))); // unrolling (k=4)
        // result = a[i-3] + x * a[i-2] + x_2 * a[i-1] + x_3 * a[i] + x_4 * result; // expanding
        // reassociation:
        tmp0 = x_3 * a[i];
        tmp1 = x_2 * a[i-1];
        tmp2 = x * a[i-2];
        tmp3 = a[i-3];
        tmp_total = (tmp0 + tmp1) + (tmp2 + tmp3);

        result = x_4 * result + tmp_total;
    }

    for (; i >= 0; i--) {
        result = a[i] + x * result;
    }

    return result;
}

double polyh_4x4(double a[], double x, long degree)
{
    double x_2 = x * x;
    double x_3 = x_2 * x;
    double x_4 = x_3 * x;
    
    long i;
    double result = a[degree];
    double result1 = 0;
    double result2 = 0;
    double result3 = 0;

    if (degree >= 3) {
        result = x_3 * a[degree];
        result1 = x_2 * a[degree-1];
        result2 = x * a[degree-2];
        result3 = a[degree-3];
    }
    
    for (i = degree - 4; i >= 3; i -=4) {
        result  = x_4 * result  + x_3 * a[i];
        result1 = x_4 * result1 + x_2 * a[i-1];
        result2 = x_4 * result2 + x   * a[i-2];
        result3 = x_4 * result3 + a[i-3];
    }

    result = result + result1 + result2 + result3;

    for (; i >= 0; i--) {
        result = a[i] + x * result;
    }

    return result;
}

double polyh_8x8(double a[], double x, long degree)
{
    double x_2 = x * x;
    double x_3 = x_2 * x;
    double x_4 = x_3 * x;
    double x_5 = x_4 * x;
    double x_6 = x_5 * x;
    double x_7 = x_6 * x;
    double x_8 = x_7 * x;
    
    long i;
    double tmp0, tmp1, tmp2, tmp3, tmp_total;
    double result = a[degree];
    double result1 = 0;
    double result2 = 0;
    double result3 = 0;
    double result4 = 0;
    double result5 = 0;
    double result6 = 0;
    double result7 = 0;

    if (degree >= 7) {
        result = x_7 * a[degree];
        result1 = x_6 * a[degree-1];
        result2 = x_5 * a[degree-2];
        result3 = x_4 * a[degree-3];
        result4 = x_3 * a[degree-4];
        result5 = x_2 * a[degree-5];
        result6 = x * a[degree-6];
        result7 = a[degree-7];
    }
    
    for (i = degree - 8; i >= 8; i -=8) {
        result  = x_8 * result  + x_7 * a[i];
        result1 = x_8 * result1 + x_6 * a[i-1];
        result2 = x_8 * result2 + x_5 * a[i-2];
        result3 = x_8 * result3 + x_4 * a[i-3];
        result4 = x_8 * result4 + x_3 * a[i-4];
        result5 = x_8 * result5 + x_2 * a[i-5];
        result6 = x_8 * result6 + x * a[i-6];
        result7 = x_8 * result7 + a[i-7];
    }

    result = result + result1 + result2 + result3 + result4 + result5 + result6 + result7;

    for (; i >= 0; i--) {
        result = a[i] + x * result;
    }

    return result;
}

void test_polyh(char* name, polyhFunc func) {
    int test_count = 5;
    double x = 1.5;
    long size = 1000000;
    long degree = size - 1;
    double* p = (double*) malloc(size * sizeof(double));
    for (long i = 1; i <= size; i++) {
        p[i-1] = (double) (i % 20);
    }
    
    // warmup
    func(p, x, degree);
    clock_t total = 0;
    for (int i = 0; i < test_count; i++) {
        clock_t start_time = clock();
        double res = func(p, x, degree);
        total += (clock() - start_time);
    }
    clock_t avg = total / test_count;
    printf("[%s]: \t\t%ld ticks\n", name, avg);
    
    free(p);
}

int main() {
    
    long degree = 10;
    double p[11] = {6, 7, 2, 5, 8, 2, 5, 1, 2, 9, 2};
    double x = 3.5;
    double res_polyh = polyh(p, x, degree);
    double res_polyh_4x1a = polyh_4x1a(p, x, degree);
    double res_polyh_4x4 = polyh_4x4(p, x, degree);
    double res_polyh_8x8 = polyh_8x8(p, x, degree);

    assert(res_polyh_4x1a == res_polyh);
    assert(res_polyh_4x4 == res_polyh);
    assert(res_polyh_8x8 == res_polyh);

    // perf test
    test_polyh("polyh", &polyh);
    test_polyh("polyh_4x1a", &polyh_4x1a);
    test_polyh("polyh_4x4", &polyh_4x4);
    test_polyh("polyh_8x8", &polyh_8x8);

    /*
    
    Results on my M1 Pro:
    
    > clang -o 18 18.c && ./18
     [polyh]: 		4788 ticks
     [polyh_4x1a]: 		1042 ticks
     [polyh_4x4]: 		961 ticks
     [polyh_8x8]: 		768 ticks
    
    6.2 times faster!

    > clang -o 18 -arch x86_64 18.c && ./18
     [polyh]: 		5121 ticks
     [polyh_4x1a]: 		1240 ticks
     [polyh_4x4]: 		1225 ticks
     [polyh_8x8]: 		761 ticks
    
    6.7 times faster!

    */

    return 0;
}