#include "csapp.h"

int main(int argc, char **argv)
{
    int n, fd;
    rio_t rio;
    char buf[MAXLINE];

    /* ADDED CODE - BEGIN */
    if (argc > 1) {
        fd = Open(argv[1], O_RDONLY, 0);
        Dup2(fd, 0);
    }
    /* ADDED CODE - END */

    Rio_readinitb(&rio, STDIN_FILENO);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
        Rio_writen(STDOUT_FILENO, buf, n);
}
