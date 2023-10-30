#include "include/csapp.h"
#include "malloc/mm.h"
#include "malloc/memlib.h"

#define TIMES 10000
#define FIRST (TIMES * 2 / 3)
#define LAST (TIMES - FIRST)

// std malloc
void test_malloc(int array[], size_t total_size)
{

    void *m_start = sbrk(0);
    size_t malloc_size = 0;
    size_t allocated_size = 0;

    clock_t start_time = clock();
    for (int i = 0; i < TIMES; i+=2) {
        void* ptr_f = malloc(i);
        void* ptr = malloc(i+1);
        free(ptr_f);

        malloc_size += i+1;
    }
    clock_t total = (clock() - start_time);

    void *m_end = sbrk(0);
    size_t heap_size = (size_t)(m_end - m_start);

    printf("\tmalloc size: %ld, heap size: %ld\n", malloc_size, heap_size);
    printf("\t%ld ticks\n", total);;
}

/* mm_malloc from problem 17 */
void test_mm_malloc(int array[], size_t total_size)
{
    mem_init();
    void* m_start = mem_sbrk(0);
    size_t malloc_size = 0;

    clock_t start_time = clock();
    for (int i = 0; i < TIMES; i+=2) {
        void* ptr_f = mm_malloc(i);
        void* ptr = mm_malloc(i+1);
        mm_free(ptr_f);

        malloc_size += i+1;
    }
    clock_t total = (clock() - start_time);

    void* m_end = mem_sbrk(0);
    size_t heap_size = (size_t)(m_end - m_start);
    
    printf("\tmalloc size: %ld, heap size: %ld\n", malloc_size, heap_size);
    printf("\t%ld ticks\n", total);;
}

int main(void)
{
    void **ptr = malloc(10 * sizeof(void *));

    mem_init();
    int array[TIMES];
    int i;
    size_t total_size = 0;
    srand(time(0));
    
    for (i = 0; i < TIMES; i++) {
        array[i] = rand() % 1000;
        total_size += array[i];
    }
    
    printf("malloc: \n");
    test_malloc(array, total_size);
    printf("\nmm_malloc: \n");
    test_mm_malloc(array, total_size);
    
    return 0;
}