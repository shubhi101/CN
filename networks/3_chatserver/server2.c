#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
int main(int argc,char *argv[])
{
	mkfifo("server2_fifo",0666);
	printf("Service 2 started\n");
	int rfd=open("server2_fifo",O_RDONLY);
	char buf[30];
	read(rfd,buf,sizeof(buf));
	// FILE* w=NULL; 
 //    w=popen("pipe2.txt", "w"); 
 //    fputs(buf, w);
	//pclose(w); 
     printf("%s\n",buf ); 
	close(rfd);
	return 0;
}