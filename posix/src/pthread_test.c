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


#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <trace.h>

#include "tests.h"

#define LARGE_THREADS_TOTAL 4
#define SMALL_THREADS_TOTAL (7)
#define JOINED_THREADS_TOTAL (3)

static void * t0(void * args);
static void * t1(void * args);

static pthread_mutex_t thread_mutex;
int pthread_test(void){
	int i;

	pthread_mutexattr_t attr;
	pthread_t t;
	pthread_attr_t pattr;
	void * ret_value;
	int args[SMALL_THREADS_TOTAL];

	if ( pthread_mutexattr_init(&attr) < 0 ){
		printf("failed to init attr (%d)\n", errno);
		return -1;
	}


	if ( pthread_mutex_init(&thread_mutex, &attr) < 0 ){
		printf("failed to init thread_mutex (%d)\n", errno);
		return -1;
	}

	printf("Test pthread_create().....");
	fflush(stdout);

	for(i=0; i < LARGE_THREADS_TOTAL; i++){

		printf("."); fflush(stdout);

		if ( pthread_attr_init(&pattr) < 0 ){
			fflush(stdout);
			perror("attr_init failed");
			return -1;
		}

		if ( i < (LARGE_THREADS_TOTAL - 1) ){
			if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED) < 0 ){
				fflush(stdout);
				perror("setdetachstate failed");
				return -1;
			}
		} else {
			if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE) < 0 ){
				fflush(stdout);
				perror("setdetachstate failed");
				return -1;
			}
		}

		if ( pthread_attr_setstacksize(&pattr, 2048) < 0 ){
			fflush(stdout);
			perror("setstacksize failed");
			return -1;
		}

		args[i] = i;
		if ( pthread_create(&t, &pattr, t0, &args[i]) < 0 ){
			perror("create failed");
			return -1;
		}
	}

	if ( pthread_join(t, NULL) < 0 ){
		perror("join failed");
		return -1;
	}

	printf("passed\n");
	printf("Test pthread_create() (small)..."); fflush(stdout);

	for(i=0; i < SMALL_THREADS_TOTAL; i++){
		if ( pthread_attr_init(&pattr) < 0 ){
			fflush(stdout);
			perror("attr_init failed");
			return -1;
		}

		if ( i < (SMALL_THREADS_TOTAL - 1) ){
			if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED) < 0 ){
				fflush(stdout);
				perror("setdetachstate failed");
				return -1;
			}
		} else {
			if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE) < 0 ){
				fflush(stdout);
				perror("setdetachstate failed");
				return -1;
			}
		}

		if ( pthread_attr_setstacksize(&pattr, 384) < 0 ){
			fflush(stdout);
			perror("setstacksize failed");
			return -1;
		}

		if ( pthread_create(&t, &pattr, t1, &args[i]) ){
			perror("create failed");
			fflush(stdout);
			return -1;
		}
	}


	if ( pthread_join(t, NULL) < 0 ){
		fflush(stdout);
		perror("join failed--");
		return -1;
	}

	printf("passed\n");

	printf("Test pthread_join()...");
	fflush(stdout);
	for(i=0; i < JOINED_THREADS_TOTAL; i++){

		if ( pthread_attr_init(&pattr) < 0 ){
			fflush(stdout);
			perror("attr_init failed");
			return -1;
		}

		if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE) < 0 ){
			fflush(stdout);
			perror("setdetachstate failed");
			return -1;
		}

		if ( pthread_attr_setstacksize(&pattr, 384) < 0 ){
			fflush(stdout);
			perror("setstacksize failed");
			return -1;
		}

		if ( pthread_create(&t, &pattr, t1, &i) ){
			fflush(stdout);
			perror("create failed");
			return -1;
		}

		if ( pthread_join(t, &ret_value) < 0 ){
			fflush(stdout);
			perror("join failed");
			return -1;
		}

		printf("r%d...", (int)ret_value);
		fflush(stdout);

		if ( (int)ret_value != i ){
			printf("failed to get return value (%d != %d)\n", (int)ret_value, args[i]);
			return -1;
		}
	}

	printf("passed\n");


	return 0;
}

void * t0(void * args){
	int * t = (int*)args;
	if ( pthread_mutex_lock(&thread_mutex) < 0 ){
		printf("t%d failed to lock mutex (%d)\n", *t, errno);
	}
	printf("t%d...", *t);
	usleep(200*1000);
	if ( pthread_mutex_unlock(&thread_mutex) < 0 ){
		printf("t%d failed to lock mutex (%d)\n", *t, errno);
	}
	fflush(stdout);
	return NULL;
}


void * t1(void * args){
	int t = *((int*)args);
	pthread_mutex_lock(&thread_mutex);
	usleep(200*1000);
	pthread_mutex_unlock(&thread_mutex);
	return (void*)(t);
}
