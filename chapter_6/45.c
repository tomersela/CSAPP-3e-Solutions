#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 16384
#define BLOCK_SIZE 8


unsigned long counter()
{
  unsigned long val;
  asm volatile("mrs %0, cntpct_el0"
               : "=r"(val));

  return val;
}

unsigned long counter_freq_mhz()
{
  unsigned long val;
  asm volatile("mrs %0, cntfrq_el0"
               : "=r"(val));
  return val / 1e6;
}

void transpose(int *dst, int *src, int dim)
{
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[j * dim + i] = src[i * dim + j];
}

void fast_transpose(int *dst, int *src, int dim)
{
    int i, j, l, m;
    int limit = dim - BLOCK_SIZE;
    int src_row;
    for (i = 0; i < limit; i += BLOCK_SIZE)
        for (j = 0; j < limit; j += BLOCK_SIZE)
        {
            int l_limit = i + BLOCK_SIZE;
            int m_limit = j + BLOCK_SIZE;

            for (l = i; l < l_limit; l++) {
                src_row = l * dim;
                for (m = j; m < m_limit; m++)
                    dst[m * dim + l] = src[src_row + m];
            }
        }

    int saved_pos = i;

    // take care of the remaining items
    for (; i <= dim - 1; i++)
        for (j = 0; j < saved_pos; j += BLOCK_SIZE)
            for (m = j; m < j + BLOCK_SIZE; m++)
                dst[m * dim + i] = src[i * dim + m];

    for (i = 0; i <= dim - 1; i++)
        for (j = saved_pos; j <= dim - 1; j++)
            dst[j * dim + i] = src[i * dim + j];
}

int main()
{
    int *src = (int *)malloc(N * N * sizeof(int));
    int *dst = (int *)malloc(N * N * sizeof(int));

    // Populate the array with values
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            src[i * N + j] = i * N + j;

    unsigned long start_time, end_time, total_ns;

    start_time = counter();
    fast_transpose(dst, src, N);
    end_time = counter();

    total_ns = (end_time - start_time) / counter_freq_mhz();
    printf("total: \t\t%ld ns\n", total_ns);

    // we must use dst, otherwise level 3 optimization of the compiler might remove the call to transpose
    printf("last element = %d\n", dst[(N + 1) * (N - 1)]);

    // validate
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            assert(dst[i * N + j] == j * N + i);

    free(src);
    free(dst);

    return 0;
}