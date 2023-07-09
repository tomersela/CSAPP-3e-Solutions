
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
    for (i = 0; i < length - 6; i += 6) {
        sum = sum + (
            udata[i] * vdata[i] +
            udata[i+1] * vdata[i+1] +
            udata[i+2] * vdata[i+2] +
            udata[i+3] * vdata[i+3] +
            udata[i+4] * vdata[i+4] +
            udata[i+5] * vdata[i+5]);
    }
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