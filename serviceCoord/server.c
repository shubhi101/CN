//reads pvtfifo from shm, writes msg to pvtfifo, sends signal to main server144
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define nClients 3
#define nServers 3
#define bufsize 500


int pvtfifo,ppid;
char resbuf[bufsize];
char *sc2s;

void serverFn(){
	printf("pvtfifo is %s\n",sc2s);
	mkfifo(sc2s,S_IRUSR|S_IWUSR);
	pvtfifo=open(sc2s,O_WRONLY);
	strcpy(resbuf,"I am a service\n");
	write(pvtfifo,resbuf,strlen(resbuf)+1);
	//signal the parent
	//which id to signal to?
	kill(ppid,SIGUSR2);
}


int main(int argc,char *argv[]){
	//attach to shared memory
	key_t key=ftok(".",'a');
	int shmid=shmget(key,20*sizeof(char),IPC_CREAT|0666);
	sc2s=shmat(shmid,NULL,0);
	//connect to signal handlers
	signal(SIGUSR1,serverFn);
	while(1){
		pause();
	}
}