#include <complex.h>

double c_imag(double complex x) {
    return cimag(x);
}

double c_real(double complex x) {
    return creal(x);
}

double complex c_sub(double complex x, double complex y) {
    return x - y;
}

/*
double c_imag(double complex x)
1	c_imag:
2	movapd	%xmm1, %xmm0
3	ret
double c_real(double complex x)
4	c_real:
5	rep; ret
double complex c_sub(double complex x, double complex y)
6	c_sub:
7	subsd	%xmm2, %xmm0
8	subsd	%xmm3, %xmm1
9	ret


A. %xmm0 & %xmm1 represents the first argument (real & imaginary respectively)
%xmm2 & %xmm3 represents the first argument (real & imaginary respectively)

B. %xmm0 & %xmm1 are used as return value (real & imaginary respectively)

*/