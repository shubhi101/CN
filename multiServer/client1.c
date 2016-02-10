#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int resfifo1,reqfifo;
char reqbuf[20];
char resbuf[100];
char sno[2];

void reqFn(){
	while(1){
		reqfifo=open("reqfifo",O_WRONLY);
		//printf("fifo opened write\n");
		strcpy(reqbuf,"resfifo1|");strcpy(sno,"");
		printf("%s\n","Which service do you want(1,2,3)?" );
		read(0,sno,2);
		strcat(reqbuf,sno);
		write(reqfifo,reqbuf,strlen(reqbuf)+1);
		printf("Sent request %s to main server\n",reqbuf);

	}
}

void serviceFn(){
	while(1){
		//printf("fifo opened read\n");
		if(read(resfifo1,&resbuf,sizeof(char)*100)>0){
			printf("Service sent %s\n",resbuf );
			strcpy(resbuf,"");
		} 
	}
}

int main(){
	mkfifo("reqfifo",S_IRUSR|S_IWUSR);
	mkfifo("resfifo1",S_IRUSR|S_IWUSR);
	resfifo1=open("resfifo1",O_RDONLY|O_NONBLOCK);
	pthread_t reqThread,serviceThread;
	pthread_create(&reqThread,NULL,(void*)&reqFn,NULL);
	pthread_create(&serviceThread,NULL,(void*)&serviceFn,NULL);
	printf("in manin\n");
	pthread_join(reqThread,NULL);
	pthread_join(serviceThread,NULL);
}
