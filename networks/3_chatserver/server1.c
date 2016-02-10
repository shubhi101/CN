#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
int main(int argc,char *argv[])
{
	mkfifo("server1_fifo",0666);
	printf("Service 1 started\n");
	int rfd=open("server1_fifo",O_RDONLY);
	char buf[30];
	read(rfd,buf,sizeof(buf));
	
    printf("%s\n",buf );
    
	close(rfd);
	return 0;
}