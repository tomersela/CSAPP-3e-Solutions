#include<assert.h>


long cread(long *xp) {
    return (xp ? *xp : 0);
}

/*
Compiled with clang -o 61 -O1 -arch x86_64 61.c into:

100003df0: 55                          	pushq	%rbp
100003df1: 48 89 e5                    	movq	%rsp, %rbp
100003df4: 48 c7 45 f8 00 00 00 00     	movq	$0, -8(%rbp)
100003dfc: 48 85 ff                    	testq	%rdi, %rdi
100003dff: 48 8d 45 f8                 	leaq	-8(%rbp), %rax
100003e03: 48 0f 45 c7                 	cmovneq	%rdi, %rax
100003e07: 48 8b 00                    	movq	(%rax), %rax
100003e0a: 5d                          	popq	%rbp
100003e0b: c3                          	retq
100003e0c: 0f 1f 40 00                 	nopl	(%rax)
*/
long cread_alt(long *xp) {
    long zero = 0;
    return *(xp ? xp : &zero);
}


int long_size_bits = sizeof(long) << 3;

/*
Not what asked, but an expiriment I did - not using cmov, but also not using jump.
compiled with `clang -o 61 -O1 -arch x86_64 61.c` into:

100003d90: 55                          	pushq	%rbp
100003d91: 48 89 e5                    	movq	%rsp, %rbp
100003d94: 48 89 f8                    	movq	%rdi, %rax
100003d97: 48 f7 d8                    	negq	%rax
100003d9a: 48 09 f8                    	orq	%rdi, %rax
100003d9d: 8a 0d 6d 42 00 00           	movb	17005(%rip), %cl        ## 0x100008010 <_long_size_bits>
100003da3: 80 c1 ff                    	addb	$-1, %cl
100003da6: 48 d3 f8                    	sarq	%cl, %rax
100003da9: 48 c7 45 f8 00 00 00 00     	movq	$0, -8(%rbp)
100003db1: 48 21 c7                    	andq	%rax, %rdi
100003db4: 48 f7 d0                    	notq	%rax
100003db7: 48 8d 4d f8                 	leaq	-8(%rbp), %rcx
100003dbb: 48 21 c1                    	andq	%rax, %rcx
100003dbe: 48 09 f9                    	orq	%rdi, %rcx
100003dc1: 48 8b 01                    	movq	(%rcx), %rax
100003dc4: 5d                          	popq	%rbp
100003dc5: c3                          	retq
100003dc6: 66 2e 0f 1f 84 00 00 00 00 00       	nopw	%cs:(%rax,%rax)
*/
long cread_alt_no_cmov_no_jmp(long *xp) {
    long temp = 0;
    long *tempp = &temp;
    unsigned long uptempp = (unsigned long) tempp;
    unsigned long uxp = (unsigned long) xp;
    
    long mask = (uxp | (~uxp + 1)); // msb of mask is zero in case xp is zero otherwise msb of mask is one
    mask = mask  >> (long_size_bits - 1); // now, mask is zero in case xp is zero, otherwise mask if full of ones
    unsigned long result = (mask & uxp) | (~mask & uptempp);

    return *(long*) result;
}

int main() {
    long num = 123;
    assert(cread_alt(0) == 0);
    assert(cread_alt(&num) == 123);

    assert(cread_alt_no_cmov_no_jmp(0) == 0);
    assert(cread_alt_no_cmov_no_jmp(&num) == 123);
    return 0;
}
