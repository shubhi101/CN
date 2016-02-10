#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
char buf[30];

int main(){
	mkfifo("c2s",S_IRUSR|S_IWUSR);
	int ffd=open("c2s",O_WRONLY);

	int n=0;
	strcpy(buf,"I am a message from Fifo");
	while(1){
		write(ffd,buf,strlen(buf)+1);
	}
}