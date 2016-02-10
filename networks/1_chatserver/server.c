#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>

int main()
{
	int wfd1,rfd1,wfd2,rfd2,wfd3,rfd3;
	char buf[30];
	int i;
	struct pollfd fds[3];

	mkfifo("c1",S_IRUSR|S_IWUSR);
	mkfifo("c2",S_IRUSR|S_IWUSR);
	mkfifo("c3",S_IRUSR|S_IWUSR);
	mkfifo("c4",S_IRUSR|S_IWUSR);
	mkfifo("c5",S_IRUSR|S_IWUSR);
	mkfifo("c6",S_IRUSR|S_IWUSR);

	wfd1=open("c2",O_WRONLY);
	rfd1=open("c1",O_RDONLY);

	fds[0].fd=rfd1;
	fds[0].events=POLLIN;

	wfd2=open("c4",O_WRONLY);
	rfd2=open("c3",O_RDONLY);

	fds[1].fd=rfd2;
	fds[1].events=POLLIN;

	wfd3=open("c6",O_WRONLY);
	rfd3=open("c5",O_RDONLY);
	
	fds[2].fd=rfd3;
	fds[2].events=POLLIN;
	printf("server started\n");
	while(1)
	{
		printf("entered into loop\n");
		i=poll(fds,3,1000);
		printf("%d\n", i);
		if(i>0)
		{
			if((fds[0].revents & POLLIN))
			{
				printf("Reading from client 1\n");
				
				read(rfd1,buf,sizeof(buf));
					write(wfd2,buf,sizeof(buf));
					write(wfd3,buf,sizeof(buf));
				//for(i=0;i<sizeof(buf);i++)
				//buf[i]=" ";
			}	
			if((fds[1].revents & POLLIN))
			{
				printf("Reading from client 2\n");
				read(rfd2,buf,sizeof(buf));
					write(wfd1,buf,sizeof(buf));
					write(wfd3,buf,sizeof(buf));
				//for(i=0;i<sizeof(buf);i++)
				//buf[i]="";
			}
			if((fds[2].revents & POLLIN))
			{	
				printf("Reading from client 3\n");	
				read(rfd3,buf,sizeof(buf));
					write(wfd1,buf,sizeof(buf));
					write(wfd2,buf,sizeof(buf));
			}
		}
	}
}
