#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
int main()
{
	mkfifo("c2s",0666);
	mkfifo("s2c1",0666);
	char buf[30];
	int c=fork();
	int rfd,wfd;
	pid_t pid=getpid();
	char fifo_name[]="s2c1";
	if(c>0)
	{
		printf("client 1 parent \n");
		while(1)
		{
			wfd=open("c2s",O_WRONLY);
			read(0,buf,2);
			int j=2,i=0;
			while(fifo_name[i]!='\0')
			{
				buf[j]=fifo_name[i];
                j++;
                i++;
			}
			write(wfd,buf,sizeof(buf));
			//write(wfd,fifo_name,sizeof(fifo_name));
			//write(wfd,pid,sizeof(pid));
		}
	}
	else
	{
		printf(" client 1 child\n");
		while(1)
		{
			rfd=open("s2c1",O_RDONLY);
			read(rfd,buf,sizeof(buf));
			printf("%s\n",buf);
		}
	}
}