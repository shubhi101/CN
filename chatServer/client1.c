#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int sc1,cs1;
char buf[100];
char msg[100];
void kbdFn(){
	while(1){
		printf("%s\n","Client 1 : What's on your mind?\n" );
		strcpy(buf,"");
		read(0,buf,100);
		write(cs1,buf,strlen(buf)+1);
	}
}

void fifoFn(){
	while(1){
		read(sc1,&msg,sizeof(char)*100);
		if(strcmp(msg,"")){
			printf("Client 1 got message : %s\n",msg);
			strcpy(msg,"");
		}
	}
}

int main(){
	mkfifo("sc1",S_IRUSR|S_IWUSR);
	mkfifo("cs1",S_IRUSR|S_IWUSR);
	
	sc1=open("sc1",O_RDONLY|O_NONBLOCK);
	printf("hi - c1\n");
	cs1=open("cs1",O_WRONLY);
	printf("hi-c1 write\n");

	pthread_t kbdThread,fifoThread;
	pthread_create(&kbdThread,NULL,(void*)&kbdFn,NULL);
	pthread_create(&fifoThread,NULL,(void*)&fifoFn,NULL);
	
	pthread_join(kbdThread,NULL);
	pthread_join(fifoThread,NULL);

}
