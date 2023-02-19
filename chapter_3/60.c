/*
long loop(long x, int n)
 x in %rdi, n in %esi
1	loop:
2	  movl	%esi, %ecx  ; save n to %ecx (%cl is part of %ecx)
3	  movl	$1, %edx    ; %edx = 1 (also means %rdx = 1)
4	  movl	$0, %eax    ; %edx = 0 (also means %rax = 0)
5	  jmp	.L2
6	.L3:
7	  movq	%rdi, %r8   ; %r8 = x
8	  andq	%rdx, %r8   ; %r8 = x & %rdx
9	  orq	%r8, %rax   ; %rax = rax | (x & %rdx)
10	  salq	%cl, %rdx   ; %rdx = %rdx << n
11	.L2:
12	  testq	%rdx, %rdx
13	  jne	.L3         ; is %rdx != 0 ?
14	rep; ret


A. Which registers hold program values x, n, result , and mask?
%rdi holds x
%ecx (%cl) holds n
%rax holds result
%rdx holds mask

B. What are the initial values of result and mask?
result = 0
mask = 1

C. What is the test condition for mask?
mask != 0

D. How does mask get updated?
mask gets shifted by n in each iteration

E. How does result get updated?
result is being set with all the bits of x which have a position that's a multiply of n

F. Fill in all the missing parts of the C code. 
*/

long loop(long x, int n)
{
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask = mask << n) {
        result	|= x & mask;
    }
    return result;
}


