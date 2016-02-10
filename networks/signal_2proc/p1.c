#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>
int *x,*y,*p1_id,*p2_id;
void inc_x()
{
signal(SIGUSR2,inc_x);
printf("incrementing x  %d \n",*x);
*x=*y+1;
// //sleep(20);

kill(*p2_id,SIGUSR1);
}

int main()
{
	 key_t k= ftok(".",'a');
	 int shmid=shmget(7207,sizeof(int),0666|IPC_CREAT);
	  x=(int*)shmat(shmid,NULL,0);
	 int shmid1=shmget(1207,sizeof(int),0666|IPC_CREAT);
	  y=(int*)shmat(shmid1,NULL,0);
     
	 int sp1_id=shmget(12345 ,sizeof(int),0666|IPC_CREAT);
	  p1_id=(int*)shmat(sp1_id,NULL,0);

     
	 int sp2_id=shmget(432198,sizeof(int),0666|IPC_CREAT);
	  p2_id=(int*)shmat(sp2_id,NULL,0);

	 *p1_id=getpid();
	 sleep(10);
	 printf("%d %d \n",*p1_id,*p2_id);
	*x=0;
	*y=0;

    	//sigaction(SIGUSR2, &act, NULL);
    	signal(SIGUSR2,inc_x);	
    	raise(SIGUSR2);
    	while(1)
    	{

    	}
    
}