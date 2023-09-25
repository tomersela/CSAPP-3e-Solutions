#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

extern char **environ;

int mysystem(char *command) {
    char* argv[] = {"", "-c", command, NULL};
    pid_t pid = 0;
    int status = 0;
    if ((pid = fork()) == 0) {
        if (execve("/bin/sh", argv, environ) < 0)
            fprintf(stderr, "error executing command %s: %s\n", command, strerror(errno));
    }

    printf("child pid: %d\n", pid);
    if (waitpid(pid, &status, 0) > 0) {
        if (WEXITSTATUS(status)) // Terminated normally
            return WEXITSTATUS(status);
        
        if (WIFSIGNALED(status)) // Terminated because of a signal
            return WTERMSIG(status);
    }

    return 0;
}

int main() {
    char* commands[] = {"sh ./22_good.sh", "sh ./22_bad.sh", "sh ./22_ugly.sh", NULL};
    int i = 0;
    for (char* command = commands[i]; command != NULL; command = commands[++i]) {
        int res;
        res = mysystem(command);
        printf("result for command '%s': %d\n", command, res);
    }
    return 0;
}
