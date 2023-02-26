/*
A.

long A[R][S][T];

Array element A[i][j][k] is at memory address:

&A[i][j][k] = XA + sizeof(long)*(S*T*i + T*j + k)

B.

long store_ele(long i, long j, long k, long *dest)
 i in %rdi, j in %rsi, k in %rdx, dest in %rcx
1	store_ele:
2	leaq	(%rsi,%rsi,2), %rax     ; %rax = 3*j
3	leaq	(%rsi,%rax,4), %rax     ; %rax = 4*%rax+%rsi = 4*3*j+j = 13*j
4	movq	%rdi, %rsi              ; %rsi = i
5	salq	$6, %rsi                ; %rsi = i*2^6 = 64*i
6	addq	%rsi, %rdi              ; %rdi = i + 64*i = 65*i
7	addq	%rax, %rdi              ; %rdi = 65*i + 13*j
8	addq	%rdi, %rdx              ; %rdx = 65*i + 13*j + k
9	movq	A(,%rdx,8), %rax        ; %rax = M[XA + %rdx*8] = M[XA + 8*(65*i + 13*j + k)]
10	movq	%rax, (%rcx)            ; *dest = M[XA + 8*(65*i + 13*j + k)]
11	movl	$3640, %eax             ; sizeof(A); means R*S*T = 3640
12	ret


According to the assembly code, the address of element A[i][j][k] is
&A[i][j][k] = XA + 8*(65*i + 13*j + k)

Using our formula from part A:
S*T = 65
13 = T
Therefore, S*13 = 65 => S = 5

Also, the assembly shows sizeof(A) compiles to 3640, therefore:
8*R*S*T = 3640
=> 8*R*65 = 3640 => R = 7


*/

#define R 7
#define S 5
#define T 13

long A[R][S][T];

long store_ele(long i, long j, long k, long *dest)
{
    *dest = A[i][j][k];
    return sizeof(A);
}

int main() {
}