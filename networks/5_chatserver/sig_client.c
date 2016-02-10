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
    	printf("do u want to send signal again  ? print y if yes else n \n");
    	scanf("%d",&ans);
    }
}