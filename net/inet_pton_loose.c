#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>


int inet_pton_loose(int family, const char* strptr, void* addrptr) {
  if (family == AF_INET) {
	struct in_addr in_val;
	if (inet_pton(AF_INET, strptr, &in_val)) {
	  memcpy(addrptr, &in_val, sizeof(struct in_addr));
	  return 1;
	}

	int result = inet_aton(strptr, &in_val);
	printf ("result:%d\n", result);
	return 0;
  }
  errno = EAFNOSUPPORT;
  return -1;
}

int main() {
  struct sockaddr_in  servaddr;
  memset(&servaddr, 0, sizeof(servaddr));  
  servaddr.sin_family = AF_INET;           
  servaddr.sin_port   = htons(13); 

  int ar = inet_pton_loose(AF_INET, "127.0.0.1", &servaddr.sin_addr);
  printf("1ar %d\n"); //成功
  ar = inet_pton_loose(AF_INET, "123", &servaddr.sin_addr);
  printf("2ar %d\n"); //成功
  ar = inet_pton_loose(AF_INET, "127.0.0.1.0", &servaddr.sin_addr);
  printf("3ar %d\n"); //失败
  ar = inet_pton_loose(AF_INET, "abc.123.sdf.123", &servaddr.sin_addr);
  printf("4ar %d\n"); //失败
  ar = inet_pton_loose(AF_INET, "123.123", &servaddr.sin_addr);
  printf("5ar %d\n"); //成功
}

