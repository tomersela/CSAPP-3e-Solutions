/*

void setVal(str1 *p, str2 *q)
 p in %rdi, q in %rsi
1	setVal:
2	movslq	8(%rsi), %rax   ; %rax = q->t
3	addq	32(%rsi), %rax  ; %rax += 
4	movq	%rax, 184(%rdi) ; 
5	ret

*/

#define A 9
#define B 5

typedef struct {
    int x[A][B]; /* Unknown constants A and B */
    long y;
} str1;

typedef struct {
    char array[B];  // 0 (up to 8 bytes)
    int t;          // 8 (4 bytes)
    short s[A];     // 12 (up to 20 bytes -> up to 10 short elements)
    long u;         // 32
} str2;

void setVal(str1 *p, str2 *q) {
    long v1 = q->t;  // movslq	8(%rsi), %rax   ; means t in on the 8th byte of the str2
    long v2 = q->u;  // addq	32(%rsi), %rax  ; means u in on the 32nd byte of str2
    p->y = v1+v2;    // movq	%rax, 184(%rdi) ; mean y is on the 184th byte of str1
}

/*

long v1 = q->t; translates to:  movslq	8(%rsi), %rax
This means t property of str2 starts at the 8th byte.
Therefore, everything before property t (including padding) takes 8 bytes.
Since t is int, it could be located in multiples of 4.
The fact that t is located on the 8th byte suggests that the size of array is 4 < B <= 8 (a)

Likewise,
long v2 = q->u; translates to:  addq	32(%rsi), %rax
It means that u property of str2 starts at the 32nd byte.
Therefore, everything before property t (including padding) takes 32 bytes.
Since t is 4 bytes, s starts at the 12 byte
We conclude that the size of array s and possible padding takes 20 bytes
and because u is long (8 bytes), 12 < sizeof(s) <= 20
s is array of type short => 6 < A <= 10 (b)


p->y = v1+v2; translates to:    movq	%rax, 184(%rdi)
This means that everything before y including padding takes 184 bytes => sizeof(x) <= 184
and becuase y is long (8 bytes) => 176 < sizeof(x) <= 184
A & B are defined in advance, therefore, the size of x array in str1 is A*B*sizeof(int)
x is an array of ints (4 bytes), therefore, 44 < A*B <= 46 (c)


So far:
(a) 4 < B <= 8
(b) 6 < A <= 10
(c) 44 < A*B <= 46  => A*B can only be 45 or 46

If A*B = 46:
We try to divide it by possible values of B (5 to 8), but none of them gives an integer number.
We can't have a portion of a byte, means A*B can't be 46


If A*B = 45:
We try to divide it by possible values of B (5 to 8), but only B = 5 gives an integer result
A*5 = 45
 => A = 9

A = 9
B = 5

*/