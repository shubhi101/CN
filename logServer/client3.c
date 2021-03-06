#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int s2c3,c2s;
char sno[2];
char reqbuf[20];
char resbuf[20];
char msgbuf[50];
int privatefifo;
int s2c3;

void reqFn(){
	printf("In reqfn\n");
	while(1){
		//request for a service
		c2s=open("c2s",O_WRONLY);
		strcpy(reqbuf,"");
		strcpy(reqbuf,"3|");
		strcpy(sno,"");
		printf("%s\n","Which service do you want(1,2,3)?" );
		read(0,sno,2);
		strcat(reqbuf,sno);
		write(c2s,reqbuf,strlen(reqbuf)+1);
		printf("Sent request %s to main server\n",reqbuf);
	}
}

void serviceFn(){
	printf("In serviceFn\n");
	while(1){
		//get private fifoname and connect to it
		if(read(s2c3,&resbuf,20*sizeof(char)+1)>0){
			printf("got private fifo name as : %s\n",resbuf );
			privatefifo=open(resbuf,O_WRONLY);
			if(privatefifo==-1) printf("error openig fifo client 3 \n");
			else printf("Succesfully opened fifo client 3\n");
			strcpy(msgbuf,"Client3 says Hello\n");
			//write msg to privatefifo
			write(privatefifo,msgbuf,strlen(msgbuf)+1);
			printf("wrote msg\n");
		}
	}
}

int main(){
	s2c3=open("s2c3",O_RDONLY|O_NONBLOCK);
	printf("read fifo opened\n");

	pthread_t reqThread,serviceThread;
	pthread_create(&reqThread,NULL,(void*)&reqFn,NULL);
	pthread_create(&serviceThread,NULL,(void*)&serviceFn,NULL);
	
	pthread_join(reqThread,NULL);
	pthread_join(serviceThread,NULL);
}
