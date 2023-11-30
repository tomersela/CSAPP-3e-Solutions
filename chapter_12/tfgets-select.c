#include "csapp.h"

#define TIMEOUT_SECONDS 5

char *tfgets(char *ptr, int n, FILE *stream) {
    char *rptr;
    fd_set ready_set;

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    FD_ZERO(&ready_set);
    FD_SET(STDIN_FILENO, &ready_set);

    Select(1, &ready_set, NULL, NULL, &timeout);
    if (FD_ISSET(STDIN_FILENO, &ready_set))
        return Fgets(ptr, n, stream);

    return NULL;
}
