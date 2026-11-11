#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: %s NUMBER\n", argv[0]);
        exit(-1);
    }
    char *ptr;
    int time = 0;
    for (ptr = argv[1]; *ptr; ptr++) {
        if (*ptr < '0' || *ptr > '9') {
            fprintf(2, "Error, expected a number, got %s\n", argv[1]);
            exit(-1);
        }
        time = (time * 10) + (*ptr - '0');
    }
    pause(time);
    exit(0);
}