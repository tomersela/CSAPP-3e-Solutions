#include "csapp.h"

void *thread(void *vargp);

int main(int argc, char *argv[])
{
    pthread_t* tids;
    int n;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);

    tids = Malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
        Pthread_create(&tids[i], NULL, thread, NULL);

    for (int i = 0; i < n; i++)
        Pthread_join(tids[i], NULL);

    exit(0);
}

void *thread(void *vargp)
{
    printf("Hello, world!\n");
    return NULL;
}
