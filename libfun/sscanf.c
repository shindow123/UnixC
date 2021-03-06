#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(){
    char text[256];
    long arg1, arg2;
    while (fgets(text, 256, stdin) > 0) {
        // "%ld%ld" 是以空格分开 "%ld:%ld" 以:分开
        if (sscanf(text, "%ld %ld", &arg1, &arg2) == 2) {
            printf("%ld,%ld\n", arg1, arg2);
        } else {
            printf("error:%s\n", strerror(errno));
        }
    }
    return 0;
}
