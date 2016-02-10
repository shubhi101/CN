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
int *msid;
int main()
{
	
	int shmid1=shmget(4326,sizeof(int),0666|IPC_CREAT);// pid of main server
    msid=(int*)shmat(shmid1,NULL,0);
    printf("my pid is %d i am sending signal to %d\n",getpid(),*msid );
    kill(*msid,SIGUSR1);
}