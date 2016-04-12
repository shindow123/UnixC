#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

// 
#include <errno.h>

const char *FILE_NAME = "/tmp/this_file_does_not_exist.yarly";

int main( int argc, char **argv )
{
      int fd = 0;

      printf( "Opening %s...\n", FILE_NAME );	
      fd = open( FILE_NAME, O_RDONLY, 0644 );
      if( fd < 0 ) { // 库函数返回-1时, 有错误发生, 设置全局变量error
		             // 当库函数没反生错误, error是无效的
					 // 注意在多线程中, 错误码是作为线程函数的返回值输出
            // Error, as expected.
            perror( "Error opening file" ); // to stderr
            printf( "Error opening file: %s\n", strerror( errno ) ); // to stdout
      }

      return EXIT_SUCCESS;
}
