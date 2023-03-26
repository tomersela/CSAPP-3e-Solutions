union ele {
	struct {
        long *p;
        long y;
	} e1;
	struct {
        long x;
        union ele *next;
	} e2;
};

void proc (union ele *up) {
	up->e2.x = *(up->e2.next->e1.p) - up->e2.next->e1.y;
}

/*
A.

e1.p	0
e1.y	8
e2.x	0
e2.next	8

B. 16

C.
void proc (union ele *up)
 up in %rdi
1	proc:
2	movq	8(%rdi), %rax   ;   if %rax points to e1 then %rax = y ; if %rax points to e2 then %rax = up->e2.next
3	movq	(%rax), %rdx    ;   %rdx = *(up->e2.next)    (y isn't a reference), at this point %rdx points to another ele type (also means that %rax = up->e2.next)
4	movq	(%rdx), %rdx    ;   %rdx = up->e2.next->e1.p (x isn't a reference), now %rdx has a long value: *(up->e2.next->e1.p)
5	subq	8(%rax), %rdx   ;   if %rax (up->e2.next) is e1 then 8(%rax) = up->e2.next->e1.y ; if %rax (up->e2.next) is e2 then 8(%rax) = up->e2.next->e2.next
                            ;   but the second option isn't possible as we learnt on line 4 that up->e2.next is an e1 type
                            ;   %rdx is now *(up->e2.next->e1.p) - up->e2.next->e1.y;
6	movq	%rdx, (%rdi)    ;   %rdi is a e2, therefore, (%rdi) = up->e2.x
7	ret
*/