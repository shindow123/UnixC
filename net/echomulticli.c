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

#define MAXLINE 1024
static int  read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];
static ssize_t
my_read(int fd, char *ptr)
{

    if (read_cnt <= 0) {
again:
        if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR)
                goto again; 
            return(-1);
        } else if (read_cnt == 0)
            return(0);
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ( (rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;  /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return(n - 1);  /* EOF, n - 1 bytes were read */
        } else
            return(-1);     /* error, errno set by read() */
    }

    *ptr = 0;   /* null terminate like fgets() */
    return(n);
}

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t     n;

    if ( (n = readline(fd, ptr, maxlen)) < 0)
        printf("readline error: %s\n", strerror(errno));
    return(n);
}

ssize_t                     /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR) {
                nwritten = 0;       /* and call write() again */
				printf("write interupt\n");
			}
            else {
                return(-1);         /* error */
            }
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}

void
Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        printf("writen error: %s\n", strerror(errno));
}

char *
Fgets(char *ptr, int n, FILE *stream)
{
    char    *rptr;

	printf("Fgets:");
    if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
        printf("fgets error: %s\n", strerror(errno));

    return (rptr);
}


int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: a.out <IPaddress>\n");
        return 0;
    }

	int i = 0;
	int sockfd[5];
	struct sockaddr_in	servaddr;
	for (i = 0; i < 5; i++) {
	  if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error: %s\n", strerror(errno));
	  }
	  memset(&servaddr, 0, sizeof(servaddr));
	  servaddr.sin_family = AF_INET;
	  servaddr.sin_port   = htons(9999);	/* daytime server */
	  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("inet_pton error for %s", argv[1]);
		return 0;
	  }

	  if (connect(sockfd[i], (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		printf("connect error: %s\n", strerror(errno));
		return 0;
	  }
	}




	char	sendline[MAXLINE], recvline[MAXLINE];
	char    *rptr;

    signal(SIGPIPE, SIG_IGN); // 如果不忽略
                              // 往已经已经接受到RST标记的SOCKET写数据
                              // 将产生SIGPIPE信号并退出游戏
	while(fgets(sendline, MAXLINE, stdin) != NULL) {
        // 如果对方SOCKET已关闭(kill服务器进程)
        // 此时将会收到一个RST标记
		if (write(sockfd[0], sendline, 1) < 0) {
			printf("write error: %s\n", strerror(errno));
        }

        sleep(1);

        // 此时会产生SIGPIPE信号
		if (write(sockfd[0], sendline+1, strlen(sendline) - 1) < 0) {
			printf("write error: %s\n", strerror(errno));
        }

		if (Readline(sockfd[0], recvline, MAXLINE) < 0) {
			printf("str_cli:server terminated prematurely (%s)\n", strerror(errno));
			//return 0;
		}

		if (fputs(recvline, stdout) == EOF)
		  printf("fputs error: %s\n", strerror(errno));
	}
    printf("over\n");

	if (ferror(stdin)) {
        printf("fgets error: %s\n", strerror(errno));
	}
	exit(0);
}
