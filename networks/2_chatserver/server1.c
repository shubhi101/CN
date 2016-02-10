#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
int main(int argc,char *argv[])
{
	mkfifo(argv[1],0666);
	printf("Service 1 started\n");
	int wfd=open(argv[1],O_WRONLY);
	char buf[]="Hi , Welcome to service 1";
	write(wfd,buf,sizeof(buf));
	close(wfd);
	return 0;
}