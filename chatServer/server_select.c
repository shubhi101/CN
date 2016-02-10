#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#define num 3
#define bufsize 500
#define timeout 1000

int main(){
	char buf[bufsize];
	fd_set rfds;
	int sc[num+1];
	int fds[num];
	struct timeval tv;
	int retval;

	mkfifo("sc1",S_IRUSR|S_IWUSR);
	mkfifo("sc2",S_IRUSR|S_IWUSR);
	mkfifo("cs1",S_IRUSR|S_IWUSR);
	mkfifo("cs2",S_IRUSR|S_IWUSR);
	mkfifo("sc3",S_IRUSR|S_IWUSR);
	// mkfifo("sc4",S_IRUSR|S_IWUSR);

	fds[0]=open("cs1",O_RDONLY|O_NONBLOCK);
	fds[1]=open("cs2",O_RDONLY|O_NONBLOCK);
	fds[2]=open("cs3",O_RDONLY|O_NONBLOCK);
	// fds[3].fd=open("cs4",O_RDONLY|O_NON_BLOCK);

	sc[1]=open("sc1",O_WRONLY);
	sc[2]=open("sc2",O_WRONLY);
	sc[3]=open("sc3",O_WRONLY);
	// sc[4]=open("sc4",O_WRONLY);

	
	

	while(1){
		FD_ZERO(&rfds);
		int i;
		int max=fds[0];
		for(i=0;i<num;i++){
			FD_SET(fds[i],&rfds);
				if(fds[i]>max)
					max=fds[i];
			}
		tv.tv_sec=1;
		tv.tv_usec=0;
		retval=select(max+1,&rfds,NULL,NULL,&tv);
		if(retval){
			int i;
			for(i=0;i<num;i++){
				if(FD_ISSET(fds[i],&rfds)){
					read(fds[i],buf,bufsize);
					printf("Server read for %d msg %s\n",i,buf );
					int j;
					for(j=1;j<=num;j++)
						if(j!=i+1)
							write(sc[j],buf,strlen(buf));
					
				}
	
			}
		}
	}
	
}
