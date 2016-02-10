#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>

int *msid,fd;
	char buf[100];
void func(){
	printf("Inside signal func\n");
	write(fd,buf,100);
	sleep(5);
	signal(SIGUSR1,func);
}

int main(int argc, char **argv){
	char msgbuf[30];
	int pfd[2],ffd,pofd;
	FILE *f, *fp;
	fd_set rfds;
	int fds[3];
	struct timeval tv;
	int retval;


	strcpy(msgbuf,"Hi, I am process \n");
	int pid1=0,pid2=0,pid3=0;

	//shared memory to share pid of the process
	int shmid = shmget(2357,sizeof(int),0666|IPC_CREAT);
    msid=(int*)shmat(shmid,NULL,0);
    *msid = getpid();
    printf("Shared memory created\n");

    //attach handler to signal
    signal(SIGUSR1,func);

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
		fds[0]=0;
		fds[1]=pfd[0];
		fds[2]=ffd;
		fds[3]=pofd;

		while(1){
			FD_ZERO(&rfds);
			int i;
			int max=fds[0];
			for(i=0;i<4;i++){
				FD_SET(fds[i],&rfds);
				if(fds[i]>max)
					max=fds[i];
			}
			tv.tv_sec=1;
			tv.tv_usec=0;
			retval=select(max+1,&rfds,NULL,NULL,&tv);
			if(retval){
				int i;
				
				if(FD_ISSET(fds[0],&rfds)){
					printf("Reading from keyboard \n");
					read(0 ,buf,sizeof(buf));
					printf("Read %s from keyboard\n",buf );
				}

				if(FD_ISSET(fds[1],&rfds)){
					printf("Reading from pipe \n");
					read(fds[1],buf,sizeof(buf));
					printf("Message sent from pipe : %s\n",buf);
					sleep(5);
				}

				if(FD_ISSET(fds[2],&rfds)){
					printf("Reading from fifo \n");
					read(fds[2],buf,sizeof(buf));
					printf("Message sent from fifo : %s\n",buf);
				}

				if(FD_ISSET(fds[3],&rfds)){
					printf("Reading from popen \n");	
					read(fds[3],buf,sizeof(buf));
					printf("Message sent from popen : %s\n",buf);
				}
			}
		}	
	}
}