#include<inttypes.h>

typedef __int128 int128_t;
void store_prod(int128_t *dest, int64_t x, int64_t y) {
    *dest = x * (int128_t) y;
}

/*
X = 2^64*Xh + Xl, Y = 2^64*Yh + Yl

X*Y = (2^64*Xh + Xl) * (2^64*Yh + Yl)
  = 2^64*Xh*2^64*Yh + 2^64*Xh*Yl + Xl*2^64*Yh + Xl*Yl
  = 2^128*Xh*Yh + 2^64*Xh*Yl + 2^64*Xl*Yh + Xl*Yl
  = 2^128*Xh*Yh + 2^64*(Xh*Yl + Xl*Yh) + Xl*Yl

Since we have only 128 bits, anything shifted by 128 will be wiped out, therefor:

  = 2^64*(Xh*Yl + Xl*Yh) + Xl*Yl

Also, any overflow of (Xh*Yl + Xl*Yh) will be wiped out, so we shouldn't worry about it in the assembly code

GCC produces the following:
;  *dest in %rdi, x in %rsi, y in %rdx

1	store_prod:
2	movq	%rdx, %rax              ; save y to %rax
3	cqto                            ; sign extend %rax (y) to %rdx:%rax (128 bits) - means %rdx is Yh and %rax is Yl
4	movq	%rsi, %rcx              ; set %rcx to x
5	sarq	$63, %rcx               ; %rcx is now all ones in case x is negative, or all zeros in case x is positive - means %rcx is now Xh

; So far Yh = %rdx, Yl = %rax, Xh = %rcx, Xl = %rsi

6	imulq	%rax, %rcx              ; %rcx = Xh*Yl
7	imulq	%rsi, %rdx              ; %rdx = Xl*Yh
8	addq	%rdx, %rcx              ; %rcx = Xh*Yl + Xl*Yh
9	mulq	%rsi                    ; multiply %rsi with %rax (Xl*Yl) and set the result in %rdx:%rax
10	addq	%rcx, %rdx              ; adding the high bits of Xl*Yl with %rdx (Xh*Yl + Yh*Xl) -> %rdx = 
11	movq	%rax, (%rdi)            ; storing low bits of Xl*Yl in low bits part of *dest
12	movq	%rdx, 8(%rdi)           ; storing Xh*Yl + Xl*Yh plus the high bits of Xl*Yl in high bit part of *dest
13	ret	
*/
