#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc != 2){
	fprintf(2, "There need a number of the sleep time!");
	exit(1);     
    }
    else{
    	int n = atoi(argv[1]);
    	sleep(n);
    	exit(0);
    }


}
