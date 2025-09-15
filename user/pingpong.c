#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main (int argc, char* argv[]) {
    int p2c[2];
    int c2p[2];
    pipe(p2c);
    pipe(c2p);
    int pid;
    char buf[4];
    if (fork() == 0) {
        // child
        close(p2c[1]);
        close(c2p[0]);
        pid = getpid();
        read(p2c[0], buf, 4);
        close(p2c[0]);
        printf("%d: received %s\n", pid, buf);
        write(c2p[1], "pong", 4);
        close(c2p[1]);
        exit(0);
    } else {
        //parent
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], "ping", 4);
        close(p2c[1]);
        wait((int *) 0);
        pid = getpid();
        read(c2p[0], buf, 4);
        printf("%d: received %s\n", pid, buf);
        close(c2p[0]);
        exit(0);
    }
}