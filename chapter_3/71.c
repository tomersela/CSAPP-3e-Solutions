#include <stdio.h>

#define BUFF_SIZE 20

void good_echo() {
    char buff[20];

    while (1) {
        char* p = fgets(buff, BUFF_SIZE, stdin);
        if (p == NULL){
            break;
        }
        printf("%s", p);
    }

}

int main() {
    good_echo();
    return 0;
}