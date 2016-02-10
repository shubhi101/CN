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
int main()
{
	mkfifo("fifo1",0666);
	int fd=open("fifo1",O_WRONLY);
	char buf[30];
	while(1)
	{
		scanf("%s",buf);
		write(fd,buf,sizeof(buf));
    }
}