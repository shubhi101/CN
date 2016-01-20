#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int fd[argc][2],i;
	for(i=0;i<argc;i++){
		int k=pipe(fd[i]);
	}
	int pid=0;
	for(i=1;i<argc;i++){
		pid=0;
		pid=fork();
		if(pid==0){	
			if(i!=argc-1){
				dup2(fd[i-1][0],0);
				dup2(fd[i][1],1);
			}
			else{
				dup2(fd[i-1][0],0);
			}
			char * text[1]={NULL};
		//	int k=execvp(argv[i],text);
			execlp(argv[i],argv[i],NULL);
			
		}	
	}
	printf("Parent\n");	
	if(pid!=0){
		dup2(fd[0][1],1);
		int k;
		scanf("%d",&k);	
		printf("%d\n",k);
	}
	wait(NULL);
	return 0;
}
