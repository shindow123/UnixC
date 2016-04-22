#include  <sys/types.h>
#include  <sys/socket.h>
#include  <time.h>
#include  <netinet/in.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <errno.h>
#include  <string.h>
#include  <unistd.h>

#define MAXLINE 1024
#define LISTENQ 1024
int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
		printf("socket error: %s\n", strerror(errno));
    }

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

    if ((bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
		printf("bind error: %s\n", strerror(errno));
        return 0;
    }

    if ((listen(listenfd, LISTENQ)) < 0) {
		printf("listen error: %s\n", strerror(errno));
        return 0;
    }

	for ( ; ; ) {
		connfd = accept(listenfd, NULL, NULL);
        if (connfd < 0) {
            printf("accept error: %s\n", strerror(errno));
            return 0;
        }

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        int i, len = strlen(buff);
        printf("len %d\n", len);
        for (i = 0; i <= len; i++) {
            printf("send %c\n", buff[i]);
            write(connfd, &buff[i], 1);
        }
        //write(connfd, buff, strlen(buff));

		close(connfd);
	}
    return 0;
}
