#include "csapp.h"

#define TIMEOUT 5
#define BUFF_SIZE 20

static sigjmp_buf timeout_jmp_buf;

void timeout_handler(int sig) {
    siglongjmp(timeout_jmp_buf, 1);
}

char *tfgets(char * restrict str, int size, FILE * restrict stream) {
    char* result = NULL;

    if (Signal(SIGALRM, timeout_handler) == SIG_ERR) // register SIGALRM handler
      unix_error("error while registering alarm handler ");
    
    alarm(TIMEOUT); // trigger SIGALRM when TIMEOUT

    if (sigsetjmp(timeout_jmp_buf, 1)) { // alarm triggered (timeout)
        return NULL;
    }

    result = fgets(str, size, stream);
    Signal(SIGALRM, SIG_DFL); // deregister our handler in case there's a result
    return result;
}

int main() {
    char buff[BUFF_SIZE];
    char* p = tfgets(buff, BUFF_SIZE, stdin);
    if (p == NULL) {
        printf("timeout!\n");
        exit(0);
    }
    printf("received: %s", p);
    return 0;
}
