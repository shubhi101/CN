#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char **argv){
	int fd[2][argc];
	int i=1;
	for(i=0;i<argc;i++){
		int k=pipe(fd[i]);
	}
	int c;

	for(i=1;i<argc;i++){
		c=fork();
		if(c==0){
			dup2(fd[i-1][0],0);
			//for tail
			if(i<argc-1)
				dup2(fd[i][1],1);
			execlp(argv[i],"",NULL);
		}
		else{
			close(fd[i-1][1]);
			close(fd[i][0]);
		}

	}

	//for head
	int n;
	dup2(fd[0][1],1);
	scanf("%d\n",&n);
	printf("%d\n",n );

	wait(NULL);
}