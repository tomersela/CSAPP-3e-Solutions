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
    "ucomiss %%xmm1, %%xmm0\n\t"
    "ja .A\n\t" // POS
    "jp .P\n\t" // NaN (OTHER)
    "je .E\n\t" // = (ZERO)
    "movl	$0, %%eax\n\t" // NEG
    "jmp .END\n\t"
    ".A:\n\t"
    "movl	$2, %%eax\n\t"
    "jmp .END\n\t"
    ".P:\n\t"
    "movl	$3, %%eax\n\t"
    "jmp .END\n\t"
    ".E:\n\t"
    "movl	$1, %%eax\n\t"
    ".END:\n\t"
    : "=r" (res)
    : "f" (x)
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
