#include <assert.h>
#include <sys/signalfd.h>
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
#define NFDS 1
int *msid;

void func()
{
	    int err;
        sigset_t sigset;
        int fd;
        int ret;
        ssize_t bytes;
        struct pollfd pfd[NFDS];
        err = sigemptyset(&sigset);
        assert(err == 0);
        err = sigaddset(&sigset, SIGUSR1);
        assert(err == 0);
        err = sigprocmask(SIG_BLOCK, &sigset, NULL);
        assert(err == 0);
        fd = signalfd(-1, &sigset, 0);
        assert(fd != -1);
        
        
        pfd[0].fd = fd;
        pfd[0].events = POLLIN | POLLERR | POLLHUP;


        for (;;) {
                printf("Waiting.\n");
                ret = poll(pfd, NFDS, -1);
                printf("I 'm awake!\n");

            
                assert(ret > 0);
                assert(pfd[0].revents & POLLIN);
				struct signalfd_siginfo info;
                bytes = read(fd, &info, sizeof(info));
                assert(bytes == sizeof(info));

                unsigned sig = info.ssi_signo;
                unsigned user = info.ssi_pid;

                if (sig == SIGUSR1)
                        printf("Got SIGUSR1 from user %u\n", user);
        }
}
int main(int argc, char* argv[])
{
        
    int shmid1=shmget(4326,sizeof(int),0666|IPC_CREAT);// pid of main server
    msid=(int*)shmat(shmid1,NULL,0);
    *msid=getpid();
    printf("my pid is %d \n",*msid);

    while(1)
    {
    	signal(SIGUSR1,func);
    }

    return 0;
}
