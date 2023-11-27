#include "csapp.h"

/*
Malicous client program
*/
int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port;
    char *buf = "I've been through the desert on a line with no end";
    rio_t rio;

    if (argc < 3)
    {
        printf("Usage: %s <host> <port>", argv[0]);
        exit(1);
    }

    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);

    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, strlen(buf));
    
    printf("Connection established, press any key to close the program...\n");
    getchar();
    
    exit(0);
}