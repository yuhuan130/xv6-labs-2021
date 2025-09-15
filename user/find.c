#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char *path, char *name);
char *add_path(char *path, char *name);

int main(int argc, char *argv[]) {
    find(argv[1], argv[2]);
    exit(0);
}

void find(char *path, char *name) {
    struct dirent de;
    struct stat st;
    int fd;
    fd = open(path, 0);
    fstat(fd, &st);
    switch(st.type) {
    case T_FILE:
        break;
    case T_DIR:
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) continue;
            if (strcmp(de.name, ".") == 0) continue;
            else if (strcmp(de.name, "..") == 0) continue;
            else {
                if (strcmp(name, de.name) == 0) {
                    printf("%s\n", add_path(path, de.name));
                }
                find(add_path(path, de.name), name);
            }
        }
    }
    close(fd);
}


char *add_path(char *path, char *name) {
    int length = strlen(path) + strlen(name) + 2;
    char *buf = malloc(length);
    memmove(buf, path, strlen(path));
    buf[strlen(path)] = '/';
    memmove(buf + strlen(path) + 1, name, strlen(name));
    buf[strlen(path) + strlen(name) + 1] = '\0';
    return buf;
}