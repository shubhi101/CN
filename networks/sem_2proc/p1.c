#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
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
   //printf("failed to semop"); 
}
int main()
{
 key_t k= ftok(".",'a');
 int semid=semget(k,1,IPC_CREAT|0666);
 semctl(semid,0,SETVAL,0);
  int semid2=semget(k,1,IPC_CREAT|0666);
 semctl(semid2,0,SETVAL,0);
 int shmid=shmget(k,sizeof(int),0666|IPC_CREAT);
 int *x=(int*)shmat(shmid,NULL,0);
 int shmid1=shmget(k,sizeof(int),0666|IPC_CREAT);
 int *y=(int*)shmat(shmid1,NULL,0);
 *x=0;
 while(*x<10)
 {
 	P(semid);
 	 *x=*y+1;
  printf("printing x %d \n",*x);
  V(semid2);
  
 
 }
}
