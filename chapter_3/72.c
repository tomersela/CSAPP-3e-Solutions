#include <alloca.h>

long aframe(long n, long idx, long *q) {
    long i;
    long **p = alloca(n * sizeof(long *));
    p[0] = &i;
    for (i = 1; i < n; i++)
        p[i] = q;
    return *p[idx];
}

long vframe(long n, long idx, long *q) {
    long i;
    long *p[n];
    p[0] = &i;
    for (i = 1; i < n; i++)
        p[i] = q;
    return *p[idx];
}

/*

long aframe(long n, long idx, long *q)
 n in %rdi, idx in %rsi, q in %rdx
1	aframe:
2	pushq	%rbp
3	movq	%rsp, %rbp
4	subq	$16, %rsp           ; Allocate space for i (%rsp = s1)
5	leaq	30(,%rdi,8), %rax   ; %rax = 8*n+30
6	andq	$-16, %rax          ; Set the 4 LSB of %rax to 0 (Round down to an address that divides with 16)
7	subq	%rax, %rsp          ; Allocate space for array p (%rsp = s2)
8	leaq	15(%rsp), %r8       ; %r8 = s2 + 15
9	andq	$-16, %r8           ; Set %r8 to &p[0]
 ⋮

|               |
|               |
|               |   <- s1
|               |   ↕ e1
|               |   <- p[n]
|               |   
|               |
|               |
|               |
|               |
|               |
|               |
|               |   <- p[0] = round_down_16(s2 + 15) = round_up_16(s2)
|               |   ↕ e2
|               |   <- s2 = s1 - round_down_16(8*n+30) = s1 - round_down_16(8*n+8*3+6) = s1 - round_down_16(8*(n+3)+6)
|               |
|               |




A. s2 = s1 - round_down_16(8*n+30)
if n is even:
    s2 = s1 - round_down_16(8*n+30) = s1 - round_down_16(8*n + 16 + 14) = s1 - (8*n + 16)
if n is odd:
    s2 = s1 - round_down_16(8*n+30) = s1 - round_down_16(8n + 8 + 16 + 6) = s1 - (8n + 8 + 16) = s1 - (8n + 24)

B. p = round_down_16(s2 + 15)
if n is even:
    p = round_down_16(s2 + 15) = round_up_16(s2) = round_up_16(s1 - (8*n + 16)) = round_up_16(s1) - (8*n + 16)
if n is odd:
    p = round_down_16(s2 + 15) = round_up_16(s2) = round_up_16(s1 - (8n + 24)) = round_up_16(s1) - (8*n + 24)



C.
Max - s1 = 64, n = 1:
    s2 = 64 - (8*1+24) = 32
    p = round_up_16(s1) - (8*n + 24) = 64 - 32 = 32
    e2 = p - s2 = 32 - 32 = 0
    &p[n] = p + 8*n = p + 8 = 32 + 8 = 40
    e1 = s1 - &p[n] = 64 - 40 = 24

Min - s1 = 49, n = 2:
    s2 = 49 - (8*2 + 16) = 49 - 32 = 17
    p = round_up_16(s1) - (8*n + 16) = 64 - 32 = 32
    e2 = p - s2 = 32 - 17 = 15
    &p[n] = p + 8*n = p + 16 = 32 + 16 = 48
    e1 = s1 - &p[n] = 49 - 48 = 1

D. We can see that s2 is computed in a way that preserves whatever offset s1 has with the nearest multiple of 16.
We can also see that p will be aligned on a multiple of 8, as is recommended for an array of 8-byte elements.

*/