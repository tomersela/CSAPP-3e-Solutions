#include <assert.h>
#include<limits.h>

typedef enum {NEG, ZERO, POS, OTHER} range_t;


range_t find_range(float x)
{
    int result;
    if (x < 0)
        result = NEG;
    else if (x == 0)
        result = ZERO;
    else if (x > 0)
        result = POS;
    else
        result = OTHER;
    return result;
}

/*
Ran compilation with:
clag -o 74 -O1 -arch x86_64 74.c
*/
range_t find_range_asm(float x)
{
    range_t res = NEG;
    __asm__("xorps %%xmm1, %%xmm1\n\t"
    "movl	$1, %%ecx\n\t" // = (ZERO)
    "movl	$2, %%edx\n\t" // POS
    "movl	$3, %%esi\n\t" // OTHER
    "movl	$0, %%eax\n\t" // NEG
    "ucomiss %%xmm1, %%xmm0\n\t"
    "cmovel	%%ecx, %%eax\n\t" // = (ZERO)
    "cmoval	%%edx, %%eax\n\t" // POS
    "cmovpl	%%esi, %%eax\n\t" // OTHER
    : "=r" (res)
    : "f" (x)
    :
    );
    return res;
}

int main(int argc, char* argv[]) {
    for (unsigned int i = 0; i != UINT_MAX; i++) {
        float f = *(float*) &i;
        assert(find_range(f) == find_range_asm(f));
    }
    
    assert(find_range_asm(0.0/0.0) == OTHER);
    assert(find_range_asm(-4.5f)== NEG);
    assert(find_range_asm(0.0f) == ZERO);
    assert(find_range_asm(4.5f) == POS);
    return 0;
}