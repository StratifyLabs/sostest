/*
Copyright 2016 Tyler Gilbert

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
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
