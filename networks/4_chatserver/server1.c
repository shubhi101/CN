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
#define count 3

int *ms_pid,shmid2,*pid_array,shmid3 ,*array;
char *fifoname;

void be_active()
{
	int shmid4=shmget(6732,sizeof(char)*30,0666|IPC_CREAT); // client sent fifoname;
    fifoname=(char*)shmat(shmid4,NULL,0);
    if(fifoname[0]=='\0')
        printf("loll\n");

    int wfd=open(fifoname,O_WRONLY);
    if(wfd!=-1)
    {
    	char buf[]="Hi , Welcome to service ";
		if((write(wfd,buf,sizeof(buf)))>0)
            printf("successfully written in the fifo \n");
		close(wfd);
    }
    int shmid1=shmget(6780,sizeof(int),0666|IPC_CREAT);
    ms_pid=(int*)shmat(shmid1,NULL,0);

     int target1= getpid();
    if(target1==pid_array[0])
        array[0] = 0;
    else if(target1 == pid_array[1])
        array[1] = 0;
    else if(target1 == pid_array[2])
        array[2]=0;

    kill(*ms_pid,SIGUSR1);
}

int main()
{
    shmid2 = shmget(6789,sizeof(int)*count,0666|IPC_CREAT);//pids of sservers
    pid_array = (int*)shmat(shmid2,NULL,0);
        pid_array[0]=getpid();

    printf("server 1 started and my pid is %d\n",pid_array[0]);
    shmid3 = shmget(6785,sizeof(int)*count,0666|IPC_CREAT);//for telling who is busy
    array = (int*)shmat(shmid3,NULL,0);
    while(1)
    { 
	signal(SIGUSR2,be_active);  
    } 
}