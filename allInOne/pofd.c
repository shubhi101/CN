#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int n;
	while(1){
		printf("This msg is from popen\n");
		sleep(2);
	}
}