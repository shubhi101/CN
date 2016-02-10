#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>

int main(){
	char msgbuf[100];
	while(1){
		read(0,msgbuf,sizeof(msgbuf));
		printf("Client got output : %s\n",msgbuf);
	}
}