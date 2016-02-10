#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int sc2,cs2;
char buf[100];
char msg[100];
void kbdFn(){
	while(1){
		printf("%s\n","Client 2 : What's on your mind?\n" );
		strcpy(buf,"");
		read(0,buf,100);
		write(cs2,buf,strlen(buf)+1);
		
	}
}

void fifoFn(){
	while(1){
		read(sc2,&msg,sizeof(char)*100);
		if(strcmp(msg,"")){
			printf("Client 2 got message : %s\n",msg);
			strcpy(msg,"");
		}
	}
}

int main(){
	
	//mkfifo("cs2",S_IRUSR|S_IWUSR);
	mkfifo("sc2",S_IRUSR|S_IWUSR);
	mkfifo("cs2",S_IRUSR|S_IWUSR);
	sc2=open("sc2",O_RDONLY|O_NONBLOCK);
	cs2=open("cs2",O_WRONLY);
	

	pthread_t kbdThread,fifoThread;
	pthread_create(&kbdThread,NULL,(void*)&kbdFn,NULL);
	pthread_create(&fifoThread,NULL,(void*)&fifoFn,NULL);

	pthread_join(kbdThread,NULL);
	pthread_join(fifoThread,NULL);

}
