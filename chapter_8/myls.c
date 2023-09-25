#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    if (execve("/bin/ls", argv, envp) < 0) {
        fprintf(stderr, "error executing ls: %s\n", strerror(errno));
        exit(1);
    }
}
