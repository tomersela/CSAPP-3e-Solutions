#include <stdio.h>

#define C 0
#define M 4
#define Y 8
#define K 12
#define BLOCK_OFFSET_BITS 5
#define SET_BITS 6
#define point_color_size (4 * sizeof(int))

typedef struct
{
    int valid;
    int tag;
} CacheLine;

CacheLine lines[64];

int access_count = 0;
int miss_count = 0;

void simulateSquareAccess(int i, int j, int offset) {
    unsigned int address = i * 16 * point_color_size + j * point_color_size + offset;
    unsigned int tag = address >> (SET_BITS + BLOCK_OFFSET_BITS);
    unsigned int setIndex = (address & 0x7FF) >> BLOCK_OFFSET_BITS;

    CacheLine* line = &lines[setIndex];
    
    int miss = !(line->valid && (line->tag == tag));
    if (miss) {
        miss_count++;
    }
    line->valid = 1;
    line->tag = tag;
    access_count++;
}

int main() {
    // initialize cache state
    for (int i = 0; i < 32; i++) {
        lines[i].valid = 0;
        lines[i].tag = 0;
    }

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            simulateSquareAccess(j, i, C);
            simulateSquareAccess(j, i, M);
            simulateSquareAccess(j, i, Y);
            simulateSquareAccess(j, i, K);
        }
    }

    printf("access count = %d\n", access_count);
    printf("miss count = %d\n", miss_count);
    printf("miss rate = %.2f%%\n", ((float) miss_count) / ((float) access_count) * 100);
}