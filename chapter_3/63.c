long switch_prob(long x, long n) {
    long result = x;
	switch(n) {
	    case 60:
	    case 62:
            result = 8 * x;
            break;
	    case 63:
            result = x >> 3;
            break;
	    case 64:
            x = (x << 4) - x;
	    case 65:
            x = x * x;
	    case 61:
        default:
            result = x + 75;
	}
	return result;
}

/*
long switch_prob(long x, long n)
  x in %rdi, n in %rsi
1	0000000000400590 <switch_prob>:
2	400590: 48 83 ee 3c	sub	$0x3c,%rsi                          ; n = n - 60
3	400594: 48 83 fe 05	cmp	$0x5,%rsi                           ; compare n with 5
4	400598: 77 29	ja	4005c3 <switch_prob+0x33>               ; if ((unsigned) n) > 5 jump to line 17
5	40059a: ff 24 f5 f8 06 40 00	jmpq	*0x4006f8(,%rsi,8)  ; jump to the right table entry according to n (60 to 65)
6	4005a1: 48 8d 04 fd 00 00 00	lea	0x0(,%rdi,8),%rax       ; result = 8*x
7	4005a8: 00
8	4005a9: c3	retq
9	4005aa: 4889f8	mov	%rdi,%rax                               ; result = x
10	4005ad: 48 c1 f8 03	sar	$0x3,%rax                           ; result = result >> 3
11	4005b1: c3	retq
12	4005b2: 4889f8	mov	%rdi,%rax                               ; result = x
13	4005b5: 48 c1 e0 04	shl	$0x4,%rax                           ; result = result << 4
14	4005b9: 4829f8	sub	%rdi,%rax                               ; result = result - x
15	4005bc: 4889c7	mov	%rax,%rdi                               ; x = result
16	4005bf: 48 0f af ff	imul	%rdi,%rdi                       ; x = x * x
17	4005c3: 48 8d 47 4b	lea	0x4b(%rdi),%rax                     ; result = x + 75
18	4005c7: c3	retq


(gdb) x/6gx 0x4006f8
0x4006f8:	0x00000000004005a1	0x00000000004005c3
0x400708:	0x00000000004005a1	0x00000000004005aa
0x400718:	0x00000000004005b2	0x00000000004005bf
*/