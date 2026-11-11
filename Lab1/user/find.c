#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

#define MAX_SIZE 1024

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void find(char *path, const char *reqfile, int execute, int argc, char **args) {
    if (strlen(path) >= MAX_SIZE) {
        fprintf(2, "find: path name too long: %s\n", path);
        return;
    }
    int fd;
    if ((fd = open(path, O_RDONLY)) == -1) {
        fprintf(2, "find: could not open %s\n", path);
        return;
    }
    int e = strlen(path);
    struct stat st;
    struct dirent de;
    if (fstat(fd, &st) == -1) {
        fprintf(2, "find: could not stat %s\n", path);
        return;
    }
    switch (st.type) {
        case T_DEVICE:
        case T_FILE:
            if (strcmp(fmtname(path), reqfile) == 0) {
                close(fd);
                if (execute) {
                    int pid = fork();
                    if (pid > 0) {
                        wait(0);
                        return;
                    }
                    else {
                        args[argc] = path;
                        // for (int i = 0; i <= argc; i++) {
                        //     printf("%s\n", args[i]);
                        // }
                        // printf("\n\n");
                        exec(args[0], args);
                        exit(-1);
                    }
                }
                else { 
                    printf("%s\n", path);
                    return;
                }
            }
            break;
        case T_DIR:
            char *p = path + strlen(path);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) continue;
                memcpy(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) continue;
                find(path, reqfile, execute, argc, args);
            }
            break;
    }
    path[e] = 0;
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || (argc > 3 && strcmp(argv[3], "-exec"))) {
        fprintf(2, "Usage: %s DIR FNAME [-exec CMD]\n", argv[0]);
        exit(-1);
    }
    int exec = 0;
    if (argc > 3) exec = 1;
    char buf[MAX_SIZE];
    strcpy(buf, argv[1]);
    buf[strlen(buf)] = 0;
    char **args = (char **) malloc(sizeof(char *) * (argc - 2));
    for (int i = 0; i < argc - 4; i++) {
        args[i] = argv[i+4];
    }
    args[argc - 3] = 0;

    find(buf, argv[2], exec, argc - 4, args);

    free(args);
    exit(0);
}