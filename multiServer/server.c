#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define num 2
#define bufsize 500

char reqbuf[bufsize];
int reqfifo;
char resfifo[20];
int sno;

int main(){
	mkfifo("reqfifo",S_IRUSR|S_IWUSR);
	reqfifo=open("reqfifo",O_RDONLY|O_NONBLOCK);

	while(1){
		strcpy(reqbuf,"");
		if(read(reqfifo,reqbuf,bufsize)>0){
			printf("read reqbuf %s\n",reqbuf);
			//process the input to get resfifo and sno
			int i=0;
			while(reqbuf[i]!='|'){
				resfifo[i]=reqbuf[i];
				i++;
			}
			resfifo[i++]='\0';
			sno=reqbuf[i]-'0'; //assume service name is one digit only
			int c=fork();
				//child
			if(c==0){
				if(sno==1){
					printf("%d\n",sno );
					if(execlp("./server1","server1",resfifo,NULL)==-1)
						printf("error creating child");
					printf("%d\n",sno );
				}
				if(sno==2){
					printf("%d\n",sno );
					if(execlp("./server2","server2",resfifo,NULL)==-1)
						printf("error creating child");
					printf("%d\n",sno );
				}
				if(sno==3){
					printf("%d\n",sno );
					if(execlp("./server3","server3",resfifo,NULL)==-1)
						printf("error creating child");
					printf("%d\n",sno );
				}
			}
			else{
				//parent
			}
		}
	}
}
