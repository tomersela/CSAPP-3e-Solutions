#include <stdlib.h>
#include <string.h>
#include <assert.h>

void *my_calloc(size_t nmemb, size_t size) {
    if (size == 0 || nmemb == 0) {
        return NULL;
    }
    size_t mul = nmemb * size;
    if (mul / size != nmemb) {
        // overflow
        return NULL;
    }

    void* allocated = malloc(mul);
    memset(allocated, 0, mul);

    return allocated;
}

int main() {
    void* p = my_calloc(4, 5);
    assert(p != NULL);
    for (int i = 0; i < 20; i++) {
        assert(((char*)p)[i] == 0);
    }
    free(p);

    // // overflow
    assert(my_calloc(SIZE_MAX, 2) == NULL);

    // nmemb = 0
    assert(my_calloc(0, 4) == NULL);

    // size = 0
    assert(my_calloc(0, 4) == NULL);
    
    return 0;
}
