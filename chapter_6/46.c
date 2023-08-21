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

void col_convert(int *G, int dim)
{
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            G[j * dim + i] = G[j * dim + i] || G[i * dim + j];
}

void fast_col_convert(int *G, int dim)
{
    int i, j, l, m, tmp;
    int limit = dim - BLOCK_SIZE;
    int src_row;
    for (i = 0; i < limit; i += BLOCK_SIZE)
        for (j = 0; j < limit; j += BLOCK_SIZE)
        {
            int l_limit = i + BLOCK_SIZE;
            int m_limit = j + BLOCK_SIZE;

            for (l = i; l < l_limit; l++) {
                src_row = l * dim;
                for (m = j; m < m_limit; m++) {
                    tmp = G[m * N + l] || G[l * N + m];
                    G[m * N + l] = tmp;
                    G[l * N + m] = tmp;
                }
                    
            }
        }

    int saved_pos = i;

    // take care of the remaining items
    for (; i <= dim - 1; i++)
        for (j = 0; j < saved_pos; j += BLOCK_SIZE)
            for (m = j; m < j + BLOCK_SIZE; m++) {
                tmp = G[m * N + i] || G[i * N + m];
                G[m * N + i] = tmp;
                G[i * N + m] = tmp;
            }

    for (i = 0; i <= dim - 1; i++)
        for (j = saved_pos; j <= dim - 1; j++) {
            tmp = G[j * N + i] || G[i * N + j];
            G[j * N + i] = tmp;
            G[i * N + j] = tmp;
        }
}

void initialize_matrix(int *arr, int dim)
{
    for (int i = 0; i <= dim - 1; i++)
        for (int j = 0; j <= dim - 1; j++)
            arr[i * dim + j] = (i * dim + j) % 3 == 0;
}

int main()
{
    int *s = (int *) malloc(N * N * sizeof(int));

    initialize_matrix(s, N);

    unsigned long start_time, end_time, total_ns;

    start_time = counter();

    fast_col_convert(s, N);

    end_time = counter();

    total_ns = (end_time - start_time) / counter_freq_mhz();
    printf("total: \t\t%ld ns\n", total_ns);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            assert(s[i * N + j] == s[i * N + j]);

    return 0;
}