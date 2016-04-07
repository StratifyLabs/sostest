
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char * argv[]){

	//wait for a signal and return the signal value

	while(1){
		sleep(1);
		printf("waiting...");
		fflush(stdout);
	}

	return 0;
}
