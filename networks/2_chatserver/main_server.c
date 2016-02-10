#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
int main()
{
	mkfifo("c2s",0666);
	int rfd=open("c2s",O_RDONLY|O_NONBLOCK);
	char buf[30];
	printf("main server started\n");
	while(1)
	{
		if(read(rfd,buf,30)>0)
		{
			int t;
			int id=buf[0]-'0';
			printf("Welcome to service center you asked for service %d \n",id);
			char fifoname[30]="";
			int j=0,i=2;
			
			while(buf[i]!='\0')
			{
				fifoname[j]=buf[i];
				j++;
				i++;
			}
	        printf("%s\n",fifoname);
			if(id==1)
			{
	            t=fork();
	            if(t==0)
	            {
				    if(execlp("./server1","server1",fifoname,NULL)==-1)
				    	printf("error occured\n");
			    }
			    else
			    {
	                
			    }
			}
			else if(id==2)
			{
				t=fork();
	            if(t==0)
	            {
				    if(execlp("./server2","server2",fifoname,NULL)==-1)
				        printf("error occurred \n"); 
			    }
			    else
			    {
	                
			    }
			}
	    }
	}
}


