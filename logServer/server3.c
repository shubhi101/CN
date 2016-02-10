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
int privatefifo3; //private fifo of server1
char msgbuf[100];
char logbuf[200];

int main(int argc,char *argv[]){
	printf("Inside server 3\n");
	mkfifo("privatefifo3",S_IRUSR|S_IWUSR);
	
	privatefifo3=open("privatefifo3",O_RDONLY|O_NONBLOCK);
	
	if(privatefifo3==-1) printf("error openig fifo server 3 \n");
	else printf("Succesfully opened fifo server3 \n");


	if(read(privatefifo3,&msgbuf,sizeof(char)*100)>0){
		//prepare log entry
		strcpy(logbuf,"[LOG]Server 3 : ");
		strcat(logbuf,msgbuf);
		//send log entry to main server
		printf("%s\n",msgbuf);
	}
	
	close(privatefifo3);
}