#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>

#define nClients 3
#define nServers 3
#define bufsize 50

int c2s;
int serverIDs[nServers][2];
char *sc2s; //shared memory to send fifonames
char reqbuf[bufsize];

void allotServerFn(){
	while(1){
		if(read(c2s,reqbuf,strlen(reqbuf)+1)>0){
			printf("read reqbuf %s\n",reqbuf);
			//write reqbuf to shm
			strcpy(sc2s,reqbuf);
			
		}
	}
}

void updateListFn(){

}


int main(){

	//COMMON FIFO
	mkfifo("c2s",S_IRUSR|S_IWUSR);
	c2s=open("c2s",O_RDONLY|O_NONBLOCK);

	//attach to shared memory
	key_t key=ftok(".",'a');
	int shmid=shmget(key,20*sizeof(char),IPC_CREAT|0666);
	sc2s=shmat(shmid,NULL,0);

	//define signal handlers
	signal(SIGUSR2,freeServer);

	int id=getpid();
	printf("my id%d\n",id);

	//CHILD SERVICES
	int i,pid;
	for(i=0;i<nServers;i++){
		pid=fork();
		if(pid==0){
			//send server's pid here
			if(execlp("./server","server",NULL)==-1)
				printf("error creating child");
		}
		else{
			serverIDs[i][0]=pid;
			serverIDs[i][1]=1;	 //1 means free, 0 busy
		}
	}

	//allot free server and update free server list
	pthread_t allotServerThread,updateListThread;
	pthread_create(&allotServerThread,NULL,(void*)&allotServerFn,NULL);
	pthread_create(&updateListThread,NULL,(void*)&updateListFn,NULL);
	pthread_join(allotServerThread,NULL);
	pthread_join(updateListThread,NULL);
}

//listen on common fifo, writes pvtfifoname in shm, sends signal to a free server
//updates free list on getting a signal from S

//how to send numbers in strings, e.g. main server's pid