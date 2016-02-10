#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	while(1)
	{
		char buf[]="Hi! This message is sent from pipe";
	    write(1,buf,sizeof(buf));
	    sleep(5);
    }
}