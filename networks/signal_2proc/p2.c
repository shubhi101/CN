#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>
int *x,*y,*p1_id,*p2_id;
 

void inc_y()
 {
 	signal(SIGUSR1,inc_y);
 	printf("increamenting y %d \n" , *y);
 	*y = *x + 1 ;
    
 	kill(*p1_id,SIGUSR2);
 }

// struct sigaction act;
//  act.sa_handler = inc_y;
//  sigaction(SIGUSR1, &act, NULL);
int main()
{
	 int shmid=shmget(7207,sizeof(int),0666);
	  x=(int*)shmat(shmid,NULL,0);

	 int shmid1=shmget(1207,sizeof(int),0666);
	 y=(int*)shmat(shmid1,NULL,0);
	
	 int sp1_id=shmget(12345,sizeof(int),0666);
	 p1_id=(int*)shmat(sp1_id,NULL,0);

	 
	 int sp2_id=shmget(432198,sizeof(int),0666);
	  p2_id=(int*)shmat(sp2_id,NULL,0);
	
	*p2_id=getpid();
	sleep(5);
	printf("%d %d \n" , *p1_id , *p2_id );
	// while(*x!=0);
	// *y=0;
	*x=0;
	*y=0;
    signal(SIGUSR1,inc_y);	
	 
    //raise(SIGUSR1);   
    while(1)
    {

    }
//    	printf("printing y %d \n", *y);
    	
	
}