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
#define num 3
#define bufsize 500
#define timeout 500

FILE *f[4];
struct pollfd fds[3];
char logbuf[30];
char reqbuf[30];
char cno; //clientno
int sno;//serviceno
int c2s;//common fifo
int s2c1,s2c2,s2c3;
int sno;
const char *privateFifos[3]={"privatefifo1","privatefifo2","privatefifo3"};

//log thread polls s1,s2,s3 and maintains a log
void logFn(){
	printf("In logfn\n");
	while(1){
			int ret=poll(fds,num,timeout);
			if(ret>0){
			//One of the clients has written something
				//printf("someone wrote somethig\n");
			int i;
			for(i=0;i<3;i++){
				if(fds[i].revents & POLLIN){
					read(fds[i].fd,logbuf,sizeof(logbuf));
					printf("Server logged : %s\n",logbuf );
				}
			}
		}
	}
}


//allots the reqd service to client and pass the corresponding fifo
void serviceFn(){
	printf("In servicefn\n");
	s2c1=open("s2c1",O_WRONLY);
	s2c2=open("s2c2",O_WRONLY);
	s2c3=open("s2c3",O_WRONLY);
	printf("opened all fifos\n");
	while(1){
		if(read(c2s,reqbuf,30)>0){
			cno=reqbuf[0]-'0';
			sno=reqbuf[2]-'0';
			printf("got request %d and %d\n",cno,sno);
			if(cno==1){
				write(s2c1,privateFifos[sno-1],strlen(privateFifos[sno-1])+1);
				printf("wrote to fifo\n");
			}
			if(cno==2){
				write(s2c2,privateFifos[sno-1],strlen(privateFifos[sno-1])+1);
				printf("wrote to fifo\n");
			}
			if(cno==3){
				write(s2c3,privateFifos[sno-1],strlen(privateFifos[sno-1])+1);
				printf("wrote to fifo\n");
			}
		}
	}
}



int main(){
	
	//popen 3 pipes to s1,s2,s3
	f[1]=popen("./server1","r");
	f[2]=popen("./server2","r");
	f[3]=popen("./server3","r");
	printf("Pipes opened\n");
	//poll on these pipe fds
	fds[0].fd=fileno(f[1]);
	fds[1].fd=fileno(f[2]);
	fds[2].fd=fileno(f[3]);

	fds[0].events = POLLIN;
	fds[1].events = POLLIN;
	fds[2].events = POLLIN;

	printf("%s\n","Server ready to poll" );

	//client-server will be common fifo
	//server-client will be private	
	mkfifo("c2s",S_IRUSR|S_IWUSR);
	mkfifo("s2c1",S_IRUSR|S_IWUSR);
	mkfifo("s2c2",S_IRUSR|S_IWUSR);
	mkfifo("s2c3",S_IRUSR|S_IWUSR);
	printf("mkfifo done\n");

	c2s=open("c2s",O_RDONLY|O_NONBLOCK); 
	printf("read opened\n");
	
	/*log thread polls s1,s2,s3 and mintains a log service thread takes resfifo name from client and allot a service*/
	pthread_t logThread,serviceThread;
	pthread_create(&logThread,NULL,(void*)&logFn,NULL);
	pthread_create(&serviceThread,NULL,(void*)&serviceFn,NULL);
	pthread_join(logThread,NULL);
	pthread_join(serviceThread,NULL);
}
