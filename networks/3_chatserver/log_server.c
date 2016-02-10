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
    mkfifo("server1_fifo",0666);
    mkfifo("server2_fifo",0666);
    mkfifo("server3_fifo",0666);
    FILE* f1=popen("./server1","w");
    FILE* f2=popen("./server2","w");
    FILE* f3=popen("./server3","w");
    int fd[3];
    fd[0]=fileno(f1);
    fd[1]=fileno(f2);
    fd[2]=fileno(f3);
    struct pollfd fds[3];
    int i=0;
    for(i=0;i<3;i++)
    	{
    		fds[i].fd=fd[i];
    		fds[i].events=POLLIN;
    	}

	int rfd=open("c2ls",O_RDONLY|O_NONBLOCK);
	char buf[30];
	printf("main server started\n");
	
		while(1)
		{
			if(read(rfd,buf,30)>0)
			{
				int t;
				int id=buf[0]-'0';
				printf("Welcome to service center you asked for service %d \n",id);
				int j=0,i=2;
				int wfd=open("ls2c",O_WRONLY);
				char fifoname[30];
				if(id==1)
				{
					write(wfd,"server1_fifo",sizeof(char)*12);
				}
				else if(id==2)
				{
					write(wfd,"server2_fifo",sizeof(char)*12);
				}
				else if(id==3)
				{
	                write(wfd,"server3_fifo",sizeof(char)*12);
				}
				 int y=poll(fds,3,1000);
		       // printf("hiii %d\n", y);
		        if(y>0)
				{
					if(fds[0].revents & POLLIN)
					{
						printf("Reading from server 1\n");
						char buff[30];
		                while(fgets(buff, sizeof(buff), f1)!=NULL){
		                          printf("1 %s \n",buff );
		                 }
					}	
					if((fds[1].revents & POLLIN))
					{
						printf("Reading from server2 2\n");
						
						char buff[30];
		                while(fgets(buff, sizeof(buff), f2)!=NULL){
		                          printf("2 %s \n",buff );
		                 }
					}
					if((fds[2].revents & POLLIN))
					{	
						printf("Reading from server 3\n");
						
				        char buff[30];
		                while(fgets(buff, sizeof(buff), f3)!=NULL){
		                          printf("3 %s \n",buff );
		                 }
					}
				}
		}
	}
}
	


