/*
1	decode2:
 x in %rdi, y in %rsi, z in %rdx
2	subq	%rdx, %rsi      ; y = y - z
3	imulq	%rsi, %rdi      ; x = y*x
4	movq	%rsi, %rax      ; res = y
5	salq	$63, %rax       
6	sarq	$63, %rax       ; now res is all ones in case lsb of y is 1 (y is odd number), otherwise all zeros (y is even)
7	xorq	%rdi, %rax      ; in case %rax is all ones -> x = ~x otherwise x = x
8	ret	
*/

long decode2(long x, long y, long z) {
    long diff = y - z;
    long res = diff * x;
    if (diff % 2) {
        // diff is odd
        return ~res;
    } else {
        // diff is even
        return res;
    }
}

long decode2_shorter(long x, long y, long z) {
    long diff = y - z;
    long res = diff * x;
    return diff % 2 ?  ~res : res;
}