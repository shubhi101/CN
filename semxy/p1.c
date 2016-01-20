#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

void P(int semid){
	struct sembuf sb;
	sb.sem_num=0;
	sb.sem_op=-1;
	sb.sem_flg=0;
	semop(semid,&sb,1);
}

void V(int semid){
	struct sembuf sb;
	sb.sem_num=0;
	sb.sem_op=1;
	sb.sem_flg=0;
	semop(semid,&sb,1);
}


int main(){
	key_t key=ftok(".",'a');
	int sem1=semget(key,1,IPC_CREAT|0666);
	int sem2=semget(key,1,IPC_CREAT|0666);
	//setting values for sem 1
	semctl(sem1, 0, SETVAL, 0);
	//setting values for sem 2
	semctl(sem2, 0, SETVAL, 0);	
	//shared memory
	int xid=shmget(key,sizeof(int),IPC_CREAT|0666);
	int yid=shmget(key,sizeof(int),IPC_CREAT|0666);
	int *x=(int*)shmat(xid,NULL,0);
	int *y=(int*)shmat(yid,NULL,0);

	*x=0;
	while(*x<10){
		P(sem1);
		printf("y is %d . P1 can write now\n",*y);
		*x=*y+1;
		printf("p1 made x as %d and signalled p2\n",*x );
		V(sem2);
	}

}