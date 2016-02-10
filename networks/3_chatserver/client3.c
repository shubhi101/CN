#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
int main()
{
	mkfifo("ls2c",0666);
	mkfifo("c2ls",0666);
	char buf[30];
	int c=fork();
	int rfd,wfd;
	if(c>0)
	{
		printf("client 3 parent \n");
		while(1)
		{
			wfd=open("c2ls",O_WRONLY);
			read(0,buf,2);
			write(wfd,buf,sizeof(buf));
		}
	}
	else
	{
		printf(" client 3 child\n");
		while(1)
		{
			rfd=open("ls2c",O_RDONLY);
			read(rfd,buf,sizeof(buf));
			char fifoname[30];
			int i=0;
		    while(buf[i]!='\0')
		    {
		    	fifoname[i]=buf[i];
		    	i++;
		    }
			printf("%s\n",fifoname);
			int wfd1=open(fifoname,O_WRONLY);
            char msg[]="thanks for your service!";
            write(wfd1,msg,sizeof(msg));
		}
	}
}