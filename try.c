#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define nClients 3
#define nServers 3
#define bufsize 500

int serverIDs[nServers];

int main(){
	int i;
	printf("I am main server before forking anything\n");
	int mainid=getpid();
	for(i=0;i<nServers;i++){
		int pid=fork();
		if(pid==0){
			//child
			printf("From child i is %d\n",i);
		}
		else{
			//parent
			serverIDs[i]=pid;
			printf("From parent i is %d\n",i);
		}
	}

	if(getpid()==mainid){
		printf("This is main server\n");
		for(i=0;i<nServers;i++)
			printf("%d\n",serverIDs[i] );
	}

	
}
