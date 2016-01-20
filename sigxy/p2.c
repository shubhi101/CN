#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>


int *x,*y,*p1,*p2;

void sety(){
	printf("in sety\n");
	*y=*x+1;
	printf("y becomes %d\n",*y );
	kill(*p1,SIGUSR2);
}

int main(){
	signal(SIGUSR1,sety);

	key_t key1=ftok(".",'a');
	key_t key2=ftok(".",'b');
	key_t key3=ftok(".",'c');
	key_t key4=ftok(".",'d');
	int xid=shmget(key1,sizeof(int),IPC_CREAT|0666);
	int yid=shmget(key2,sizeof(int),IPC_CREAT|0666);
	int pid1=shmget(key3,sizeof(int),IPC_CREAT|0666);
	int pid2=shmget(key4,sizeof(int),IPC_CREAT|0666);
	
	x=(int*)shmat(xid,NULL,0);
	y=(int*)shmat(yid,NULL,0);
	p1=(int*)shmat(pid1,NULL,0);
	p2=(int*)shmat(pid2,NULL,0);

	*p2=getpid();
	while(*x<10)
		pause();

}