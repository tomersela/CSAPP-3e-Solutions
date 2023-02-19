/* Enumerated type creates set of constants numbered 0 and upward */
typedef enum {MODE_A, MODE_B, MODE_C, MODE_D, MODE_E} mode_t;

long switch3(long *p1, long *p2, mode_t action)
{
    long result = 0;
 	switch(action) {
        case MODE_A:
            result = *p2;
            *p2 = *p1;
            break;

        case MODE_B:
            result = *p1 + *p2;
            *p1 = result;
            break;

        case MODE_C:
            *p1 = 59;
            result = *p2;
            break;

        case MODE_D:
            *p1 = *p2;
            result = 27;
            break;

        case MODE_E:
            result = 27;
            break;

        default:
            result = 12;
    }

    return result;
}

/*
p1 in %rdi, p2 in %rsi, action in %edx
1	.L8:	MODE_E
2	movl	$27, %eax
3	ret
4	.L3:	MODE_A
5	movq	(%rsi), %rax
6	movq	(%rdi), %rdx
7	movq	%rdx, (%rsi)
8	ret
9	.L5:	MODE_B
10	movq	(%rdi), %rax
11	addq	(%rsi), %rax
12	movq	%rax, (%rdi)
13	ret
14	.L6:	MODE_C
15	movq	$59, (%rdi)
16	movq	(%rsi), %rax
17	ret
18	.L7:	MODE_D
19	movq	(%rsi), %rax
20	movq	%rax, (%rdi)
21	movl	$27, %eax
22	ret
23	.L9:	default
24	movl	$12, %eax
25	ret
*/