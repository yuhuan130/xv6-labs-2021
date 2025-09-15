#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"




int main(int argc, char *argv[]) {
    char *buf[MAXARG];
    int k = 0;
    for (int i = 0; i < argc; i++) {
        buf[k++] = argv[i];
    }
    // read a word by a time
    char c;
    int a = 0;
    char word[128];
    while (read(0, &c, 1) > 0) {
        if ((c == ' ' || c == '\n') && a > 0) {
            char new_word[128];
            word[a] = '\0';
            strcpy(new_word, word);
            buf[k++] = new_word;
            a = 0;
        } else {
            word[a++] = c;
        }
    }
    buf[k] = 0;
    exec(buf[0], buf);
}