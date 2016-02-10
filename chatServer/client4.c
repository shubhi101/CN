#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int sc4,cs4;
char buf[100];
char msg[100];

void kbdFn(){
	while(1){
		printf("%s\n","Client 1 : What's on your mind?\n" );
		strcpy(buf,"");
		scanf("%s",buf);
		write(cs4,buf,strlen(buf)+1);
	}
}

void fifoFn(){
	while(1){
		printf("%s\n","Client 1 : Reading from fifo\n" );
		read(sc4,&msg,sizeof(char)*100);
		if(strcmp(msg,"")){
			printf("Client 1 got message : %s\n",msg);
		}
	}
}

int main(){
	
	mkfifo("cs4",S_IRUSR|S_IWUSR);
	cs4=open("cs4",O_WRONLY);
	sc4=open("sc4",O_RDONLY);

	pthread_t kbdThread,fifoThread;
	pthread_create(&kbdThread,NULL,(void*)&kbdFn,NULL);
	pthread_create(&fifoThread,NULL,(void*)&fifoFn,NULL);

	pthread_join(kbdThread,NULL);
	pthread_join(fifoThread,NULL);

}
