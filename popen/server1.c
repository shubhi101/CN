#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int privatefifo1; //private fifo of server1
char msgbuf[100];
char logbuf[200];

int main(int argc,char *argv[]){
	
	if(read(privatefifo1,&msgbuf,sizeof(char)*100)>0){
		strcpy(logbuf,"[LOG]Server 1 : ");
		//strcat(logbuf,msgbuf);
		//send log entry to main server
		printf("%s\n",msgbuf);
	}
}