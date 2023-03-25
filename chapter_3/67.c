typedef struct {
    long a[2];
    long *p;
} strA;

typedef struct {
    long u[2];
    long q;
} strB;

strB process(strA s) {
    strB r;
    r.u[0] = s.a[1];
    r.u[1] = s.a[0];
    r.q = *s.p;
    return r;
}

long eval(long x, long y, long z) {
    strA s;
    s.a[0] = x;
    s.a[1] = y;
    s.p = &z;
    strB r = process(s);
    return r.u[0] + r.u[1] + r.q;
}

/*
strB process(strA s)
1	process:
2	movq	%rdi, %rax
3	movq	24(%rsp), %rdx ; %rdx = &z
4	movq	(%rdx), %rdx   ; %rdx = z
5	movq	16(%rsp), %rcx ; %rcx = y
6	movq	%rcx, (%rdi)   ; (%rdi) = y
7	movq	8(%rsp), %rcx  ; %rcx = x
8	movq	%rcx, 8(%rdi)  ; 8(%rdi) = x
9	movq	%rdx, 16(%rdi) ; 16(%rdi) = z
10	ret

long eval(long x, long y, long z)
 x in %rdi, y in %rsi, z in %rdx
1	eval:
2	subq	$104, %rsp
3	movq	%rdx, 24(%rsp)  ; 24(%rsp) = z
4	leaq	24(%rsp), %rax  ; %rax = &z
5	movq	%rdi, (%rsp)    ; (%rsp) = x
6	movq	%rsi, 8(%rsp)   ; 8(%rsp) = y
7	movq	%rax, 16(%rsp)  ; 16(%rsp) = &z
8	leaq	64(%rsp), %rdi
9	call	process
10	movq	72(%rsp), %rax
11	addq	64(%rsp), %rax
12	addq	80(%rsp), %rax
13	addq	$104, %rsp
14	ret


A.

    |         |
104 |         |
    |         |
    |         |    
    |    z    |
    |    x    |
64  |    y    |  <- %rdi
    |         |
    |         |
    |         |
    |         |
24  |<   z   >|
16  |<   &z  >|
8   |<   y   >|
0   |<   x   >|  <- %rsp

B. eval passes a value within %rdi - a pointer to a new address in the stack - %rsp+64


C. By references based on the stack pointer (%rsp)

D. By references based on %rdi

E. 

    |         |
104 |         |
96  |         |
88  |         |
80  |    z    |
72  |    y    |
64  |    &z   |  <- %rdi
56  |         |
48  |         |
40  |         |
32  |         |
24  |<   z   >|
16  |<   &z  >|
8   |<   y   >|
0   |<   x   >|  <- %rsp in eval
-8  |         |  <- %rsp in process

F. Caller allocats space in stack and pass the address to callee, callee stores data in the space and return the address to caller.

*/