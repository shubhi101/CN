#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	int wfd,rfd,c;
	mkfifo("c1",0666);
	mkfifo("c2",0666);
	char buf[30];
	c=fork();
	if(c>0)
	{
		printf("client 1 parent \n" );
		while(1)
		{
			wfd=open("c1",O_WRONLY);
			read(0,buf,30);
			write(wfd,buf,sizeof(buf));	
		}
	}
	else
	{
		printf(" client 1 child\n");
		while(1)
		{
			rfd=open("c2",O_RDONLY);
			read(rfd,buf,sizeof(buf));
			printf("%s\n",buf);
		}
	}
	return 0;
}
