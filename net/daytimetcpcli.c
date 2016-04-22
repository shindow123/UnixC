#include  <sys/types.h>
#include  <sys/socket.h>
#include  <time.h>
#include  <netinet/in.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <errno.h>
#include  <arpa/inet.h>
#include  <string.h>
#include  <unistd.h>

#define MAXLINE 1024
int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2) {
		printf("usage: a.out <IPaddress>\n");
        return 0;
    }

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error: %s\n", strerror(errno));
    }

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("inet_pton error for %s", argv[1]);
        return 0;
    }

	if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		printf("connect error: %s\n", strerror(errno));
        return 0;
    }

    int cnt = 0;
    printf("while\n");
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        cnt++;
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) {
            printf("fputs error: %s\n", strerror(errno));
            return 0;
        }
	}

    printf("cnt:%d\n", cnt);

	if (n < 0) {
        printf("read error, cnt %d: %s\n", cnt, strerror(errno));
        return 0;
    }

	exit(0);
}
