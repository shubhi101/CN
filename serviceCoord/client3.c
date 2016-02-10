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
#define bufsize 50

int c2s;int n;
int privatefifo3;	 //pvt fifo of client 1
char reqbuf[bufsize];//will contain the name of private fifo
char resbuf[bufsize];//will read msg from the server 1
 
//wait for user input and requests the main server
void reqFn(){
	while(1){
		printf("enter 1 if you need a service\n");
		scanf("%d",&n);
		if(n==1){
			mkfifo("privatefifo3",S_IRUSR|S_IWUSR);
			privatefifo3=open("privatefifo3",O_RDONLY|O_NONBLOCK);
			strcpy(reqbuf,"privatefifo3");
			write(c2s,reqbuf,strlen(reqbuf)+1);
		}
	}
}

//takes response from private fifo and displays onto screen
void resFn(){
	while(1){
		if(read(privatefifo3,&resbuf,sizeof(char)*50)>0){
			printf("server sent %s\n",resbuf );
		}
	}
}

int main(){
	c2s=open("c2s",O_WRONLY);

	pthread_t reqThread,resThread;
	pthread_create(&reqThread,NULL,(void*)&reqFn,NULL);
	pthread_create(&resThread,NULL,(void*)&resFn,NULL);
	
	pthread_join(reqThread,NULL);
	pthread_join(resThread,NULL);

}