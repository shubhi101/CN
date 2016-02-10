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
int *ms_pid;
int main()
{
	int shmid1=shmget(6780,sizeof(int),0666|IPC_CREAT);
    ms_pid=(int*)shmat(shmid1,NULL,0);
    
    mkfifo("c2s4",0666);
    char buf[30];
	int c=fork();
	int rfd,wfd;
	mkfifo("s2c2",0666);
	char fifo_name[]="s2c2";
	if(c>0)
	{
		printf("client 1 parent \n");
		while(1)
		{
			wfd=open("c2s4",O_WRONLY);
			write(wfd,fifo_name,sizeof(fifo_name));
			kill(*ms_pid,SIGUSR2);  //sending signal to main server;
		}
	}
	else
	{
		printf(" client 1 child\n");
		while(1)
		{
			rfd=open("s2c2",O_RDONLY|O_NONBLOCK);
			if((read(rfd,buf,sizeof(buf)))>0)
			  printf(" %s\n",buf);
		}
	}
    
}