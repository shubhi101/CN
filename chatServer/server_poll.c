#include <stdio.h>
#include <unistd.h>
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
	int sc[num+1];
	struct pollfd fds[num];

	mkfifo("sc1",S_IRUSR|S_IWUSR);
	mkfifo("sc2",S_IRUSR|S_IWUSR);
	mkfifo("cs1",S_IRUSR|S_IWUSR);
	mkfifo("cs2",S_IRUSR|S_IWUSR);
	mkfifo("sc3",S_IRUSR|S_IWUSR);
	// mkfifo("sc4",S_IRUSR|S_IWUSR);

	fds[0].fd=open("cs1",O_RDONLY|O_NONBLOCK);
	fds[1].fd=open("cs2",O_RDONLY|O_NONBLOCK);
	fds[2].fd=open("cs3",O_RDONLY|O_NONBLOCK);
//  fds[3].fd=open("cs4",O_RDONLY|O_NON_BLOCK);

	sc[1]=open("sc1",O_WRONLY);
	sc[2]=open("sc2",O_WRONLY);
	sc[3]=open("sc3",O_WRONLY);
	// sc[4]=open("sc4",O_WRONLY);

	fds[0].events = POLLIN;
	fds[1].events = POLLIN;
	fds[2].events = POLLIN;
	// fds[3].events = POLLIN;

	
	
	printf("%s\n","Server ready to poll" );
		while(1){
			int ret=poll(fds,num,timeout);
			if(ret>0){
			//One of the clients has written something
			int i;
			for(i=0;i<num;i++){
				if(fds[i].revents & POLLIN){
					read(fds[i].fd,buf,bufsize);
					printf("Server read for %d msg %s\n",i,buf );
					int j;
					for(j=1;j<=num;j++)
						if(j!=i+1){
							write(sc[j],buf,strlen(buf));
						}
				}
			}
		}
	}
}
