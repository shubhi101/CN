#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <poll.h>

int privatefifo1; //private fifo of server1
char msgbuf[100];
char logbuf[200];

int main(int argc,char *argv[]){
	printf("In server 1\n");
	printf("some more text\n");
	mkfifo("privatefifo1",S_IRUSR|S_IWUSR);
	printf("made fifo\n");
	privatefifo1=open("privatefifo1",O_RDONLY|O_NONBLOCK);
	
	if(privatefifo1==-1) printf("error openig fifo server 1 \n");
	else printf("Succesfully opened fifo server1 \n");
	
	if(read(privatefifo1,&msgbuf,sizeof(char)*100)>0){
		//prepare log entry
		strcpy(logbuf,"[LOG]Server 1 : ");
		strcat(logbuf,msgbuf);
		//send log entry to main server
		printf("%s\n",msgbuf);
	}
	
	close(privatefifo1);
}