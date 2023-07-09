#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Basic implementation of memset */
void* basic_memset(void *s, int c, size_t n) {
  size_t cnt = 0;
  unsigned char *schar = s;
  while (cnt < n) {
    *schar++ = (unsigned char) c;
    cnt++;
  }
  return s;
}

void *memset2(void *s, int c, size_t n)
{
    int k = sizeof(unsigned long);
    unsigned long crep = (unsigned long) c & 0xff; // take the low significant byte of c
    for (int i = 0; i < k; i++) // replicate the lowest byte of crep k times
        crep |= crep << 8;

    size_t cnt = 0;
    size_t limit = n < k ? 0 : n - k;
    unsigned long *sulong = (unsigned long *) s;
    while (cnt < limit) {
        *sulong++ = crep;
        cnt += k;
    }

    unsigned char *schar = (unsigned char *) sulong;
    while (cnt < n) {
        *schar++ = (unsigned char) c;
        cnt++;
    }

    return s;
}

void test(size_t size, int c) {
    void* space1 = malloc(size);
    void* space2 = malloc(size);
    basic_memset(space1, c, size);
    memset2(space2, c, size);

    assert(memcmp(space1, space2, size) == 0);

    free(space1);
    free(space2);
}

int main () {
    int c = 0xAA;
    test(1000, c);
    test(8, c);
    test(2, c);
}