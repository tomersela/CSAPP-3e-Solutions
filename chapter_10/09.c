#include "csapp.h"

int main(int argc, char** argv, char **envp) {
    pid_t pid = 0;
    if ((pid = Fork()) == 0) { /* child */
        /* What code is the shell executing right here? */
        Dup2(0, 3);
        Execve("fstatcheck", argv, envp);
    }


    Waitpid(pid, NULL, 0);
}