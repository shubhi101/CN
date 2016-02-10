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

int privatefifo2; //private fifo of server1
char msgbuf[100];
char logbuf[200];

int main(int argc,char *argv[]){
	printf("In server 2\n");
	mkfifo("privatefifo2",S_IRUSR|S_IWUSR);
	privatefifo2=open("privatefifo2",O_RDONLY|O_NONBLOCK);

	if(privatefifo2==-1) printf("error opening fifo server 2 \n");
	else printf("Succesfully opened fifo server2 \n");


	if(read(privatefifo2,&msgbuf,sizeof(char)*100)>0){
		//prepare log entry
		strcpy(logbuf,"[LOG]Server 2 : ");
		strcat(logbuf,msgbuf);
		//send log entry to main server
		printf("%s\n",msgbuf);
	}
	
	close(privatefifo2);
}