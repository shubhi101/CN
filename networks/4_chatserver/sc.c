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
#define max_server 3
int semid, shmid, shmid1, shmid2, shmid3;
int count=3,*array,*pid_array;
int *ms_pid;

void P(int i)
{
     struct sembuf sb;
     sb.sem_num=0;
     sb.sem_op=-1;
     sb.sem_flg=0;
     semop(i,&sb,1); 
}

void V(int i)
{ 
     struct sembuf sb;
     sb.sem_num=0;
     sb.sem_op=1;
     sb.sem_flg=0;
     semop(i,&sb,1);
}

void request_service()//client sends sigusr2 to main server
{
	printf("service requested\n");
	signal(SIGUSR2,request_service);
	P(semid);
    int target;
    if(array[0]==0)
    {
        array[0]=1;
        target = pid_array[0];
    }
    else if(array[1]==0)
    {
        array[1]=1;
        target = pid_array[1];
    }
    else if(array[2]==0)
    {
        array[2]=1;
        target = pid_array[2];
    }
    printf("sent signal to targeted server %d \n",target );
	  kill(target,SIGUSR2);
}

void complete_service()//server sends sigusr1 to main server
{
	printf("service completed \n");
	signal(SIGUSR1,complete_service);
	V(semid);
}

int main()
{
    shmid1=shmget(6780,sizeof(int),0666|IPC_CREAT);// pid of main server
    ms_pid=(int*)shmat(shmid1,NULL,0);
    *ms_pid=getpid();

    shmid2 = shmget(6789,sizeof(int)*count,0666|IPC_CREAT);//pids of sservers
    pid_array = (int*)shmat(shmid2,NULL,0);
    int i=0;
    for(i=0 ;i <3 ;i++)
        printf("%d \n",pid_array[i] );

    shmid3 = shmget(6785,sizeof(int)*count,0666|IPC_CREAT);//for telling who is busy
    array = (int*)shmat(shmid3,NULL,0);
    for(i=0 ; i<count ; i++)
        array[i]=0;
    
  	mkfifo("c2s4",0666);
  	key_t k= ftok(".",'d');
  	char buf[30];
    semid=semget(k,1,IPC_CREAT|0666);//semaphore for no of servers
    semctl(semid,0,SETVAL,max_server);

    char *fifoname;
    shmid=shmget(6732,sizeof(char)*30,0666|IPC_CREAT);// sharing fifoname
    fifoname=(char*)shmat(shmid,NULL,0);
    printf("fifoname %s\n", fifoname);

    while(1)
    {
    	int rfd=open("c2s4",O_RDONLY);
    	if(read(rfd,buf,30)>0)
    	{
          printf("%s\n",buf );
          fifoname=buf;
          printf(" fifoname %s\n",fifoname );
          signal(SIGUSR2,request_service);
    	}
        signal(SIGUSR1,complete_service);
    }
    
}