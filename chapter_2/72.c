/*
A. the condition in the code in the quesion is always true since the type returned from the sizeof function is unsigned.
Therefore, `bytes-sizeof(val)` yields an unsigned number which is always >= 0
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void copy_int(int val, void *buf, int maxbytes) {
    if ((int) sizeof(val) <= maxbytes)
        memcpy(buf, (void *) &val, sizeof(val));
}

int main() {
    void* buffer = malloc(sizeof(int));

    int val = 0x12345678;
    copy_int(val, buffer, sizeof(int));
    assert(*(int*)buffer == val);

    int val2 = 0x87654321;
    copy_int(val, buffer, 0);
    assert(*(int*)buffer != val2);

    free(buffer);
    return 0;
}