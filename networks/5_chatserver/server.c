#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
int *msid,*pipe_fd;
char buf[30];

void func()
{
	signal(SIGUSR1,func);
	FILE *fp=popen("./client","w");
	int fd=fileno(fp);
	write(fd,buf,30);
	//strcpy(buf,"");
	sleep(5);
    //	write(buf,sizeof(buf),1,fp);
}

int main()
{
	int shmid = shmget(2357,sizeof(int),0666|IPC_CREAT);// pid of server
    msid=(int*)shmat(shmid,NULL,0);
    *msid = getpid();
    printf("main server started \n");
    int pfd[2];
    
    int t=pipe(pfd);
    int y=fork();
    if(y>0)
    {
	    close(pfd[1]);
		struct pollfd fds[4];
		int fd[4];

		fd[0]=0;
	    fd[1]=pfd[0];//pipe fd

	    mkfifo("fifo1",0666);
		fd[2]=open("fifo1",O_RDONLY);

	    FILE *fp=popen("./popen","r");
		fd[3]=fileno(fp);
	    
		int i=0;
		for(i=0;i<4;i++)
		{
			fds[i].fd=fd[i];
			fds[i].events=POLLIN;
		}
		i=0;
		signal(SIGUSR1,func);
		while(1)
		{
			//printf("entered into loop\n");
			i=poll(fds,4 ,1000);
			printf("%d\n", i);
			if(i>0)
			{
				if((fds[0].revents & POLLIN))
				{
					printf("Reading from keyboard \n");
					read(0 ,buf,sizeof(buf));
				}	
				if((fds[1].revents & POLLIN))
				{
					printf("Reading from pipe \n");
					read(fds[1].fd,buf,sizeof(buf));
					sleep(5);
					
				}
				if((fds[2].revents & POLLIN))
				{	
					printf("Reading from fifo \n");
					read(fds[2].fd,buf,sizeof(buf));
					
				}
				if((fds[3].revents & POLLIN))
				{	
					printf("Reading from popen \n");	
					read(fds[3].fd,buf,sizeof(buf));
					
				}
			}
			
		}
   }
   else
   {
			   	close(pfd[0]);
			   	dup2(pfd[1],1);
			   	execlp("./pipe","pipe",NULL);
   }
	return 0;
}