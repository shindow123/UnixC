#include  <sys/types.h>
#include  <sys/socket.h>
#include  <sys/wait.h>
#include  <time.h>
#include  <netinet/in.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <errno.h>
#include  <arpa/inet.h>
#include  <string.h>
#include  <unistd.h>
#include  <sys/select.h>

#define MAXLINE 1024

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		//maxfdp1 = max(fileno(fp), sockfd) + 1;
		maxfdp1 = sockfd + 1;
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
		  printf("select error:%s\n", strerror(errno));
		}

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (read(sockfd, recvline, MAXLINE) <= 0) {
				printf("str_cli: server terminated prematurely (%s)\n", strerror(errno));
			}
			fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			write(sockfd, sendline, strlen(sendline));
		}
	}
}

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: a.out <IPaddress>\n");
        return 0;
    }

	int i = 0;
	int sockfd;
	struct sockaddr_in	servaddr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	  printf("socket error: %s\n", strerror(errno));
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(9999);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
	  printf("inet_pton error for %s", argv[1]);
	  return 0;
	}

	if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
	  printf("connect error: %s\n", strerror(errno));
	  return 0;
	}
	
	str_cli(stdin, sockfd);		/* do it all */
    printf("over\n");
	return 0;
}
