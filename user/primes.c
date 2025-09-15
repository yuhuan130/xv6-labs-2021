#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void first_chain(void);
void chain(void);

int new_pipe[2];
int pid;
int num;

int main (int argc, char *argv[]) {
    first_chain();
    chain();
    exit(0);
}

void first_chain() {
    pipe(new_pipe);
    pid = fork();
    if (pid != 0) {
        close(new_pipe[0]);
        for (int i = 2; i < 36; i++) {
            write(new_pipe[1], &i, sizeof(int));
        }
        close(new_pipe[1]);
        exit(0);
    }
}

void chain() {
    while (1) {
        int first;
        if (pid == 0 && read(new_pipe[0], &num, sizeof(int)) != 0) {
            first = num;
            printf("prime %d\n", num);
            close(0);
            dup(new_pipe[0]);
            close(new_pipe[0]);
            close(new_pipe[1]);
            pipe(new_pipe);
            pid = fork();
            if (pid != 0) {
                close(new_pipe[0]);
                while (read(0, &num, sizeof(int)) != 0) {
                    if (num % first != 0) {
                        write(new_pipe[1], &num, sizeof(int));
                    }
                }
                close(new_pipe[1]);
                wait(0);
                exit(0);
            }
        } else {
            close(new_pipe[0]);
            close(new_pipe[1]);
            exit(0);
        }
    }
}