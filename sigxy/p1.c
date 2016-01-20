#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

int *x,*y,*p1,*p2;
void setx(){
	printf("in setx\n");
	*x=*y+1;
	printf("x becomes %d\n",*x );
	if(*x<10){
		kill(*p2,SIGUSR1);
	}
}


int main(){
	//shared memory
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

	signal(SIGUSR2,setx);

	*p1=0;*p2=0;*x=0;
	*p1=getpid();
	while(*p2==0);
	kill(*p2,SIGUSR1);
	while(*x<10)
		pause();
}