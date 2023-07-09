
typedef float data_t;

/* Create abstract data type for vector */
typedef struct {
    long len;
    data_t *data;
} vec_rec, *vec_ptr;

/* Inner product. Accumulate in temporary */
void inner4(vec_ptr u, vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;
    for (i = 0; i < length; i++) {
        sum = sum + udata[i] * vdata[i];
    }
    *dest = sum;
}

data_t *get_vec_start(vec_ptr v) {
    return v->data;
}

/* Return length of vector */
long vec_length(vec_ptr v)
{
    return v->len;
}

/*
Inner loop of inner4. data_t = double, OP = * 
    udata in %rbp, vdata in %rax, sum in %xmm0, i in %rcx, limit in %rbx

1	.L15:	loop:
2	vmovsd 0(%rbp,%rcx,8), %xmm1	    Get udata[i]
3	vmulsd (%rax,%rcx,8), %xmm1, %xmm1	Multiply by vdata[i]
4	vaddsd %xmm1, %xmm0, %xmm0	        Add to sum
5	addq $1, %rcx	                    Increment i
6	cmpq %rbx, %rcx	                    Compare i:limit
7	jne .L15	                        If ! =, goto loop 
*/