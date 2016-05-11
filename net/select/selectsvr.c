#include  <sys/types.h>
#include  <sys/wait.h>
#include  <sys/socket.h>
#include  <sys/select.h>
#include  <netinet/in.h>
#include  <time.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <errno.h>
#include  <string.h>
#include  <unistd.h>
#include  <signal.h>



#define MAXLINE 1024
#define LISTENQ 1024

typedef void Sigfunc(int);

Sigfunc *
signal(int signo, Sigfunc *func)
{
  struct sigaction    act, oact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;   /* SunOS 4.x */
#endif
  } else {
#ifdef  SA_RESTART
	act.sa_flags |= SA_RESTART;     /* SVR4, 44BSD */
#endif
  }
  if (sigaction(signo, &act, &oact) < 0)
	return(SIG_ERR);
  return(oact.sa_handler);
}

Sigfunc *
Signal(int signo, Sigfunc *func)    /* for our signal() function */
{
  Sigfunc *sigfunc;

  if ( (sigfunc = signal(signo, func)) == SIG_ERR)
	printf("signal error (%s)\n", strerror(errno));
  return(sigfunc);
}

void
str_echo(int sockfd) {
  ssize_t	n;
  char		buf[MAXLINE];
again:
  while ((n = read(sockfd, buf, MAXLINE)) > 0) {
	write(sockfd, buf, n);
	printf("recv:%s\n", buf);
  }

  if (n < 0 && errno == EINTR) {
	goto again;
  } else if (n < 0) {
	printf("str_echo: read error (%s)\n", strerror(errno));
  }
}

void
sig_chld(int signo) {
  pid_t	pid;
  int	stat;
  // WNOHANG 有尚未终止的子进程的在运行时不要阻塞 此时返回0
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
	printf("child %d terminated\n", pid);
  }
  if (pid < 0) {
	printf("sig child waitpid: %s\n", strerror(errno));
  }
  return;
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

	printf("listen!\n");
    if ((listen(listenfd, LISTENQ)) < 0) {
		printf("listen error: %s\n", strerror(errno));
        return 0;
    }
	
	int i, n, maxfd, maxi, nready, client[FD_SETSIZE], sockfd;
	fd_set rset, allset;
	maxfd = listenfd;
	maxi = -1;		/* index into client[] array */

	for (i = 0; i < FD_SETSIZE; i++) {
	  client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for (;;) {
		rset = allset;
		nready = select(maxfd+1, &rset, 0, 0, NULL);
		if (FD_ISSET(listenfd, &rset)) {
		  clilen = sizeof(cliaddr);
		  connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		  if (connfd < 0) {
			printf("accept error: %s\n", strerror(connfd));
		  }
		  printf("accept %d\n", connfd);

		  for (i = 0; i < FD_SETSIZE; i++) {
			if (client[i] < 0) {
			  client[i] = connfd;
			  break;
			}
		  }

		  if (i == FD_SETSIZE) {
			printf("too many clients\n");
		  }
			
		  FD_SET(connfd, &allset);
		  if (connfd > maxfd)
			maxfd = connfd;
		  if (i > maxi)
			maxi = i;
		  if (--nready <= 0)
			continue;
		}
		
		for (i = 0; i <= maxi; i++) {
		  if ((sockfd = client[i]) < 0) 
			continue; 

		  if (FD_ISSET(sockfd, &rset)) {
			printf("receive:");
			if ((n = read(sockfd, buff, MAXLINE)) == 0) {
			  close(sockfd);
			  FD_CLR(sockfd, &allset);
			  client[i] = -1;
			} else{
			  printf("%s\n", buff);
			  write(sockfd, buff, n);
			}

			if (--nready <= 0)
			  break;
		  }
		}
	}




	// 注册信号处理函数
	//Signal(SIGCHLD, sig_chld);

	/*
	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
			// 如果父进程在accept阻塞时候
			// 捕获到SIGCHILD(也可能是其他的)信号
			// 那么这里会产生一个EINTR错误
			// 正确操作应该是自动重启被调中断的系统调用
			// 方法1 重新调用accept
			// 方法2 设置信号的sa_flags |= SA_RESTART
            printf("accept error: %s\n", strerror(errno));
			if (errno == EINTR) {
			  continue;
			}
            //return 0;
        }

		if ((childpid = fork()) == 0) {
		  close(listenfd);
		  str_echo(connfd);
		  exit(0);
		}

		close(connfd);
	}
	*/
    return 0;
}


