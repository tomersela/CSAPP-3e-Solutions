#include "csapp.h"

sigjmp_buf timeout_jmp_buf;

#define TIMEOUT_SECONDS 5

void sigchld_handler(int sig) {
    int olderrno = errno;
    siglongjmp(timeout_jmp_buf, 1);
    errno = olderrno;
}

char *tfgets(char *ptr, int n, FILE *stream)
{
    char *rptr;
    pid_t pid;

    if (sigsetjmp(timeout_jmp_buf, 1)) {
        return NULL;
    }
    Signal(SIGCHLD, sigchld_handler);

    if ((pid = fork()) == 0) {
        Sleep(TIMEOUT_SECONDS);
        exit(0);
    }

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
        app_error("Fgets error");

    return rptr;
}