/*
1	long sum_col(long n, long A[NR(n)][NC(n)], long j) {
2	    long i;
3	    long result = 0;
4	    for (i = 0; i < NR(n); i++)
5	        result += A[i][j];
6	    return result;
7	}


long sum_col(long n, long A[NR(n)][NC(n)], long j)
  n in %rdi, A in %rsi, j in %rdx
1	sum_col:
2	leaq	1(,%rdi,4), %r8         ; %r8 = 4*n+1
3	leaq	(%rdi,%rdi,2), %rax     ; %rax = 2*n+n = 3*n
4	movq	%rax, %rdi              ; %rdi = 3*n
5	testq	%rax, %rax              ; if %rax == 0
6	jle	.L4
7	salq	$3, %r8                 ; %r8 = %r8 << 3 = %r8 * 8 = (4*n+1) * 8 = 32*n+8
8	leaq	(%rsi,%rdx,8), %rcx     ; %rcx = A + 8*j
9	movl	$0, %eax                ; %rax = 0
10	movl	$0, %edx                ; %rdx = 0
11	.L3:
12	addq	(%rcx), %rax            ; %rax += M[A + 8*j]
13	addq	$1, %rdx                ; %rdx += 1
14	addq	%r8, %rcx               ; %rcx += 32*n+8
15	cmpq	%rdi, %rdx              ; compare %rdi (3*n) with %rdx
16	jne	.L3
17	rep;	ret
18	.L4:
19	movl	$0, %eax                ; return 0
20	ret

#define NR(x) 3*(x)
#define NC(x) 4*(x)+1
*/