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
int *ms_pid, shmid2 ,*pid_array,*array,shmid3;
void be_active()
{
    printf("signal handled by server 3 \n");
	int shmid=shmget(6732,sizeof(char)*30,0666|IPC_CREAT); // client sent fifoname;
    char* fifoname=(char*)shmat(shmid,NULL,0);
    mkfifo(fifoname,0666);
    int wfd=open(fifoname,O_WRONLY);
    if(wfd!=-1)
    {
    	char buf[]="Hi , Welcome to service ";
		write(wfd,buf,sizeof(buf));
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
    printf("server 3 started and my pid is %d\n",pid_array[2]);
    shmid2 = shmget(6785,sizeof(int)*count,0666|IPC_CREAT);//pids of sservers
    pid_array = (int*)shmat(shmid2,NULL,0);
        pid_array[2]=getpid();

    shmid3 = shmget(6789,sizeof(int)*count,0666|IPC_CREAT);//for telling who is busy
    array = (int*)shmat(shmid3,NULL,0);

	while(1)
    { 
    signal(SIGUSR2,be_active);  
    } 
}