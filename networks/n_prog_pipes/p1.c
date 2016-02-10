#include <stdio.h>
#include <stdib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>
int main()
{
	int c=1;
	printf("%d\n",c);
}