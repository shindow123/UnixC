#include <stdio.h>
#include <string.h>

int main() {
    char a[] = "123456789";
    char b[25] = "opqrstu";
    char* c = "abcdefghijklmnzywvx";
    char  d = 'k';
    char* e = "54321";
    strcpy(b, c);
    printf("%s, %s\n",a, b);
    //重叠
    strcpy(b,&b[3]);
    printf("%s, %s\n",a, b);

    strcpy(b,b);
    printf("%s, %s\n",a, b);

    memcpy(b, c, 10);
    printf("%s, %s\n",a, b);

    memcpy(b, &b[3], 10);
    printf("%s, %s\n",a, b);

    memcpy(b, b, 10);
    printf("%s, %s\n",a, b);

    memset(b, 0, 50);
    return 0;
}
