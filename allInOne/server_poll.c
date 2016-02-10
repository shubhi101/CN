#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
char msgbuf[30];
char buf[100];
int pfd[2],ffd,pofd;
FILE *f, *fp;
int *msid,fd; //fd stores fd of sigclient


void func(){
	printf("Inside signal func\n");
	write(fd,buf,100);
	sleep(5);
	signal(SIGUSR1,func);
}

int main(int argc, char **argv){
	strcpy(msgbuf,"Hi, I am process \n");
	int pid1=0,pid2=0,pid3=0;

	//shared memory to share pid of the process
	int shmid = shmget(2357,sizeof(int),0666|IPC_CREAT);
    msid=(int*)shmat(shmid,NULL,0);
    *msid = getpid();

    printf("Shared memory created\n");

	//fifofd
	mkfifo("c2s",S_IRUSR|S_IWUSR);
	ffd=open("c2s",O_RDONLY|O_NONBLOCK);
	printf("fifo created\n");

	//popenfd
	f=popen("./pofd","r");
	pofd=fileno(f);
	printf("Popen created\n");

	//for client to send output to on getting a signal
	fp=popen("./client","w");
	fd=fileno(fp);

	//pipe
	pipe(pfd);

	pid1=fork();
	if(pid1==0){
		//child
		printf("In child process\n");
		close(pfd[0]);
	   	dup2(pfd[1],1);
	   	execlp("./pipefd","pipefd",NULL);
	}

	else{
		close(pfd[1]);
		struct pollfd fds[4];
		fds[0].fd=0;
		fds[1].fd=pfd[0];
		fds[2].fd=ffd;
		fds[3].fd=pofd;

		int i=0;
		for(i=0;i<4;i++)
			fds[i].events=POLLIN;
		signal(SIGUSR1,func);

		while(1)
		{
			//printf("entered into loop\n");
			i=poll(fds,4 ,1000);
			printf("%d\n", i);
			if(i>0)
			{
				if((fds[0].revents & POLLIN))
				{
					printf("Reading from keyboard \n");
					read(0 ,buf,sizeof(buf));
					printf("Read %s from keyboard\n",buf );
				}	
				if((fds[1].revents & POLLIN))
				{
					printf("Reading from pipe \n");
					read(fds[1].fd,buf,sizeof(buf));
					printf("Message sent from pipe : %s\n",buf);
					sleep(5);
					
				}
				if((fds[2].revents & POLLIN))
				{	
					printf("Reading from fifo \n");
					read(fds[2].fd,buf,sizeof(buf));
					printf("Message sent from fifo : %s\n",buf);
				}
				if((fds[3].revents & POLLIN))
				{	
					printf("Reading from popen \n");	
					read(fds[3].fd,buf,sizeof(buf));
					printf("Message sent from popen : %s\n",buf);
				}
			}
			
		}
	}


}