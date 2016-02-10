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
	char buff[30];
	while(1)
	{
		printf("client got the output\n");
		read(0,buff,sizeof(buff));
	    printf("%s\n",buff );
	}
	return 0;
}