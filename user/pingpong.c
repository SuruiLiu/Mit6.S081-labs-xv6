#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
	int p1[2], p2[2];
	char buffer[] = {'A'};
	
	long length = sizeof(buffer);
	
	pipe(p1);
	pipe(p2);
	
	if(fork() == 0){
		if(read(p1[0], buffer, length) != length){
			printf("p->c read error!");
			exit(1);
		}
		
		printf("%d: received ping\n", getpid());
		
		if(write(p2[1], buffer, length) != length){
			printf("c->p write error!");
			exit(1);
		}
		exit(0);
	}
	
	if(write(p1[1], buffer, length) != length){
		printf("p->c write error!");
		exit(1);
	}
	
	if(read(p2[0], buffer, length) != length){
		printf("c->p read error!");
		exit(1);
	}
	
	printf("%d: received pong\n", getpid());
	
	wait(0);
	exit(0);
	
}

