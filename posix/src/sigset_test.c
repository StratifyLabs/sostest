/*
 * sigset_test.c
 *
 *  Created on: Jan 19, 2012
 *      Author: tgil
 */



#include <stdio.h>
#include <signal.h>
#include "tests.h"



int sigset_test(void){
	sigset_t set;
	int i;

	printf("Test sigemptyset()...");
	sigemptyset(&set);
	if ( set != 0 ){
		printf("failed\n");
		return -1;
	}
	printf("passed\n");

	for(i=0; i < 32; i++){
		printf("Test sigaddset(%02d)...", i);

		if ( sigismember(&set, i) ){
			printf("failed (already set)\n");
			return -1;
		}

		sigaddset(&set, i);

		if ( sigismember(&set, i) ){
			printf("0x%08X passed\n", (unsigned int)set);
		} else {
			printf("failed\n");
			return -1;
		}
	}

	for(i=0; i < 32; i++){
		printf("Test sigdelset(%02d)...", i);

		if ( !sigismember(&set, i) ){
			printf("failed (already cleared)\n");
			return -1;
		}

		sigdelset(&set, i);

		if ( sigismember(&set, i) ){
			printf("failed\n");
			return -1;
		} else {
			printf("0x%08X passed\n", (unsigned int)set);
		}
	}

	printf("Test sigfillset()...");
	sigfillset(&set);
	if ( (int)set != -1 ){
		printf("failed\n");
		return -1;
	}
	printf("passed\n");

	return 0;
}
