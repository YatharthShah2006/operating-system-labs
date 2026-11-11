#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_SIZE 1024

int delim(char x) {
    if (strchr(" .\n\r\t/,-", x)) return 1;
    else return 0;
}

int digit(char x) {
    if (x < '0' || x > '9') return 0;
    else return 1;
}

enum STATUS {
    valid,
    invalid
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(2, "Usage: %s FILE1 FILE2 ...\n", argv[0]);
        exit(-1);
    }
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            printf("File %s not found\n", argv[i]);
            continue;
        }
        
        enum STATUS status = valid;
        char curr[MAX_SIZE];
        char *ptr = curr;
        int sum = 0;

        char buf[2];

        while(read(fd, buf, 1) == 1) {
            if (status == valid) {
                if (delim(*buf)) {
                    *ptr = '\0';
                    if (ptr != curr && (((sum % 3 == 0) && ((*(ptr-1) - '0') % 2 == 0)) || (*(ptr - 1) - '0') % 5 == 0)) {
                        char *tmp = curr;
                        while(((*tmp) == '0')) tmp++;
                        if (tmp == ptr) tmp--;
                        printf("%s\n", tmp);
                    }
                    ptr = curr;
                    sum = 0;
                }
                else if (digit(*buf)) {
                    *ptr = *buf;
                    sum += *buf - '0';
                    ptr++;
                }
                else {
                    status = invalid;
                }
            }
            else {
                if (delim(*buf)) {
                    ptr = curr;
                    status = valid;
                    sum = 0;
                }
            }
        }

        if (status == valid) {
            *ptr = '\0';
            if (ptr != curr && (((sum % 3 == 0) && ((*(ptr-1)) % 2 == 0)) || (*(ptr - 1)) % 5 == 0)) {
                char *tmp = curr;
                while(((*tmp) == '0')) tmp++;
                if (tmp == ptr) tmp--;
                printf("%s\n", tmp);
            }
            ptr = curr;
        }
    }
    exit(0);
}