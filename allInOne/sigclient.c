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
	int shmid = shmget(2357,sizeof(int),0666|IPC_CREAT);
    msid=(int*)shmat(shmid,NULL,0);
    int ans=1;
    while(ans==1)
    {
    	kill(*msid,SIGUSR1);
    	printf("Enter '1' to send signal again.'0' to stop. \n");
    	scanf("%d",&ans);
    }
}