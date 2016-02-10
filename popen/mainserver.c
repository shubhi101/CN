#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int main(){
	//popen 3 pipes to s1,s2,s3
	File *f=popen("./server1","w");
	int fd;
	//poll on these pipe fds
	
	
}