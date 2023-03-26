#define CNT 7

typedef struct {
    long idx;
    long x[4];
} a_struct;

typedef struct {
    int first;          // 0x0          (4 bytes)
    a_struct a[CNT];    // ?            (up to 284 bytes)
    int last;           // 0x120 = 288  (4 bytes)
} b_struct;

void test(long i, b_struct *bp)
{
    int n = bp->first + bp->last;
    a_struct *ap = &bp->a[i];
    ap->x[ap->idx] = n;
}

/*
void test(long i, b_struct *bp)
 i in %rdi, bp in %rsi
1	0000000000000000 <test>:
2	0: 8b 8e 20 01 00 00	mov	0x120(%rsi),%ecx
3	6: 030e	add	(%rsi),%ecx                         ; %ecx = bp->first + bp->last
4	8: 48 8d 04 bf	lea	(%rdi,%rdi,4),%rax          ; %rax = 5*i
5	c: 48 8d 04 c6	lea	(%rsi,%rax,8),%rax          ; %rax = %rsi + 5*i*8 = %rsi + 40*i = bp + 40*i = &bp->a[i] ;   (ap)
6	10: 48 8b 50 08	mov	0x8(%rax),%rdx              ; %rdx = (%rsi + 40*i + 8) = bp + 40*i + 8 ; (ap->idx)
7	14: 48 63 c9	movslq	%ecx,%rcx               ; %rcx = bp->first + bp->last
8	17: 48 89 4c d0 10	mov	%rcx,0x10(%rax,%rdx,8)  ; 0x10(%rax,%rdx,8) = ap + 0x10 + %rdx*8 = ap + 16 + (ap->idx)*8 = ap->x[ap->idx]
9	1c: c3	retq

A. From line 5 of the assembly code, we conclude that each element in array a is of 40 bytes in size.
Line 6 suggests that idx is located at the 8th byte of a_struct
Line 8 suggests that the x propery located at the 16th byte in a_struct

the address of ap (a_struct*) should by a multiplication of 8 (because of x, an array of longs, located at 16 byte offset)
therefore, the location of a, the second property of b_struct is 8.
Considering the address of last (0x120 - 288), we can conclude that the size of a is 280 bytes.
sizeof(a_struct) = 40, therefore sizeof(a) = 280/40 = 7
 => CNT = 7

B. Since sizeof(a_struct) = 40, and the location of the x property is 16, then sizeof(x) = 40 - 16 = 32
x is an array of longs, therefore, there are 32/8 = 4 elements in the array.
Also, idx is the first property as its index in the struct is 8 bytes.

typedef struct {
    long idx;
    long x[4];
} a_struct;

*/