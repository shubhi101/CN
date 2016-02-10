#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int resfifo;
char resbuf[100];

int main(int argc,char *argv[]){
	printf("%s\n","s2" );
	resfifo=open(argv[1],O_WRONLY);
	strcpy(resbuf,"I am server2\n");
	write(resfifo,resbuf,strlen(resbuf)+1);
	close(resfifo);
}