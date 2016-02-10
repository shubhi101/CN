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
	while(1)
	{
		char buf[]="hello!! Input from pipe";
	    write(1,buf,sizeof(buf));	
	    sleep(5);
    }
}