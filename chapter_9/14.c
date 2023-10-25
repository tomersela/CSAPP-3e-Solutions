
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    FILE *file;
    char *filename = "./hello.txt";

    int file_descriptor = open(filename, O_RDWR, 0);
    
    if (file_descriptor == -1) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    struct stat file_info;
    if (fstat(file_descriptor, &file_info) == -1) {
        perror("Error getting file information");
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }
    
    char *mapped_area = mmap(NULL, file_info.st_size, PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (((long)mapped_area) == -1) {
        printf("ERROR: Failed to map file\n");
        exit(1);
    }
    
    mapped_area[0] = 'J';

    munmap(mapped_area, file_info.st_size);
    close(file_descriptor);

    return 0;
}