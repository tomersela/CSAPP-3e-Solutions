#include "csapp.h"
#include <assert.h>
#include <time.h>

#define N 1024
#define M 1024
#define THREAD_CNT 32

typedef struct {
    int rows;
    int cols;
    int **m;
} matrix;

typedef struct {
    matrix* a;
    matrix* b;
    matrix* res;
    int row_offset;
    int row_cnt;
} matrix_mult_sub_task_params;

matrix *new_matrix(int rows, int cols);
void free_matrix(matrix *mat);
void rand_matrix(matrix *m);
void print_matrix(matrix *m);
matrix *matrix_mult(matrix *a, matrix *b);
matrix *pt_matrix_mult(matrix *a, matrix *b, int threads_num);
static int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p);

int main() {
    matrix *nm_matrix = new_matrix(N, M);
    matrix *mn_matrix = new_matrix(M, N);

    rand_matrix(nm_matrix);
    rand_matrix(mn_matrix);

    struct timespec start, end;

    printf("Testing sequential matrix multiplication...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    matrix *res_matrix = matrix_mult(nm_matrix, mn_matrix);

    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t sequentialTimeElapsed = timespecDiff(&end, &start);

    float sequentialSecs = ((float) sequentialTimeElapsed) / 1000000;
    printf("Sequential matrix multiplication took %.2f seconds (%llu microseconds).\n", sequentialSecs, sequentialTimeElapsed);
    printf("\n");

    printf("Testing threaded matrix multiplication...\n");
    for (int thread_cnt = 1; thread_cnt <= 32; thread_cnt *= 2) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        matrix *t_res_matrix = pt_matrix_mult(nm_matrix, mn_matrix, thread_cnt);

        clock_gettime(CLOCK_MONOTONIC, &end);
        uint64_t threadedTimeElapsed = timespecDiff(&end, &start);
        float threadedSecs = ((float) threadedTimeElapsed) / 1000000;
        printf ("[%d threads] %.2f seconds (%llu microseconds).\n", thread_cnt, threadedSecs, threadedTimeElapsed);
        free_matrix(t_res_matrix);
    }

    free_matrix(nm_matrix);
    free_matrix(mn_matrix);
    free_matrix(res_matrix);

    printf("\n");
}

matrix *new_matrix(int rows, int cols) {
    matrix *mat = (matrix *) Malloc(sizeof(matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->m = Malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        mat->m[i] = (int *) Malloc(cols * sizeof(int));
    
    return mat;
}

void free_matrix(matrix *mat) {
    for (int i = 0; i < mat->rows; i++)
        free(mat->m[i]);
    free(mat);
}

static void init_rand(void) {
    srand(time(NULL));
}

void rand_matrix(matrix *mat) {
    static pthread_once_t once_control = PTHREAD_ONCE_INIT;
    pthread_once(&once_control, init_rand);

    for (int i = 0; i < mat->rows; i++)
        for (int j = 0; j < mat->cols; j++)
            mat->m[i][j] = rand() % 100;
}

void print_matrix(matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols - 1; j++) {
            printf("%d, ", mat->m[i][j]);
        }
        printf("%d", mat->m[i][mat->cols - 1]);
        printf("\n");
    }
}

matrix *matrix_mult(matrix *a, matrix *b) {
    if (a->cols != b->rows) {
        fprintf(stderr, "matrix sizes are incompatible for multiplication");
        return NULL;
    }
    
    matrix *res = new_matrix(a->rows, b->cols);

    for (int i = 0; i < a->rows; i++)
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++)
                sum += a->m[i][k] * b->m[k][j];

            res->m[i][j] = sum;
        }
    return res;
}


static int pt_matrix_mult_sub_task(void *vargp) {
    matrix_mult_sub_task_params *params = (matrix_mult_sub_task_params *) vargp;
    matrix *a = params->a;
    matrix *b = params->b;
    matrix *res = params->res;
    int row_offset = params->row_offset;
    int row_cnt = params->row_cnt;

    int limit = row_offset + row_cnt;
    for (int i = row_offset; i < limit; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++)
                sum += a->m[i][k] * b->m[k][j];

            res->m[i][j] = sum;
        }
    }
}

matrix *pt_matrix_mult(matrix *a, matrix *b, int threads_num) {
    if (a->cols != b->rows) {
        fprintf(stderr, "matrix sizes are incompatible for multiplication");
        return NULL;
    }

    matrix *res = new_matrix(a->rows, b->cols);
    int rows_per_thread = a->rows / threads_num;
    int extra_rows = a->rows % threads_num;

    pthread_t* tasks_tids = Malloc(threads_num * sizeof(pthread_t));
    for (int i = 0, t = 0; i < a->rows; i += rows_per_thread, t++) {
        matrix_mult_sub_task_params *params = Malloc(sizeof(matrix_mult_sub_task_params));
        params->a = a;
        params->b = b;
        params->res = res;
        params->row_offset = i;
        params->row_cnt = a->rows < i + rows_per_thread ? a->rows - i : rows_per_thread;
        Pthread_create(&tasks_tids[t], NULL, pt_matrix_mult_sub_task, (void *) params);
    }
    
    for (int t = 0; t < threads_num; t++)
        Pthread_join(tasks_tids[t], NULL);
    
    Free(tasks_tids);
    return res;
}

static int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p) {
  return (((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec)) / 1000;
}
