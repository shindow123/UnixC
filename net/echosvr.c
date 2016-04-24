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

void
str_echo(int sockfd) {
  ssize_t	n;
  char		buf[MAXLINE];
again:
  while ((n == read(sockfd, buf, MAXLINE)) > 0) {
	write(sockfd, buf, n);
  }

  if (n < 0 && errno == EINTR) {
	goto again;
  } else if (n < 0) {
	printf("str_echo: read error (%s)\n", strerror(errno));
  }
}


int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr, cliaddr;
	char				buff[MAXLINE];
	time_t				ticks;
	pid_t				childpid;
	socklen_t			clilen;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
		printf("socket error: %s\n", strerror(errno));
    }

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(9999);	/* daytime server */

    if ((bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
		printf("bind error: %s\n", strerror(errno));
        return 0;
    }

    if ((listen(listenfd, LISTENQ)) < 0) {
		printf("listen error: %s\n", strerror(errno));
        return 0;
    }

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
            printf("accept error: %s\n", strerror(errno));
            return 0;
        }

		if ((childpid = fork()) == 0) {
		  close(listenfd);
		  str_echo(connfd);
		  exit(0);
		}

		close(connfd);
	}
    return 0;
}


