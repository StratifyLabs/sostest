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
#include "tests.h"

static void * t0(void * args);
static int recursive_lock_test();

static pthread_mutex_t mutex_var;


int pthread_mutex_test(){
	pthread_mutexattr_t mattr;
	if ( pthread_mutexattr_init(&mattr) < 0 ){
		perror("pthread_mutexattr_init() failed");
		return -1;
	}

	if ( pthread_mutex_init(&mutex_var, &mattr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( recursive_lock_test() < 0 ){
		return -1;
	}


	printf("Test pthread_mutex_trylock()...");
	if ( pthread_mutex_trylock(&mutex_var) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_mutex_unlock()...");
	if ( pthread_mutex_unlock(&mutex_var) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_mutex_trylock()...");
	fflush(stdout);

	if ( create_thread(t0, 2048, PTHREAD_CREATE_DETACHED) < 0){
		return -1;
	}

	usleep(100*1000); //wait until 50 gets the lock

	if ( pthread_mutex_trylock(&mutex_var) == 0 ){
		printf("Should have failed\n");
		return -1;
	}
	if ( errno != EBUSY ){
		printf("Should have failed with EBUSY not %d\n", errno);
		return -1;
	}
	errno = 0;
	printf("passed\n");

	printf("Test pthread_mutex_lock()...");
	fflush(stdout);
	if ( pthread_mutex_lock(&mutex_var) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	if ( pthread_mutex_unlock(&mutex_var) < 0 ){
		perror("pthread_mutex_unlock() failed");
		return -1;
	}

	if ( pthread_mutex_destroy(&mutex_var) < 0 ){
		perror("pthread_mutex_destroy() failed");
		return -1;
	}

	return 0;
}

int recursive_lock_test(){
	int total_locks;
	int i;
	pthread_mutexattr_t mattr;
	pthread_mutex_t mut;

	if ( pthread_mutexattr_init(&mattr) < 0 ){
		perror("pthread_mutexattr_init() failed");
		return -1;
	}

	if ( pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE) < 0 ){
		perror("pthread_mutexattr_settype() failed\n");
		return -1;
	}

	printf("Test pthread_mutex_init()...");
	if ( pthread_mutex_init(&mut, &mattr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_mutex_lock() (recursive)...");
	total_locks = 0;
	while(1){
		if ( pthread_mutex_lock(&mut) < 0 ){
			if ( errno == EAGAIN ){
				break;
			} else {
				fflush(stdout);
				perror("failed");
				return -1;
			}
		}
		total_locks++;
	}

	for(i=0; i < total_locks; i++){
		if ( pthread_mutex_unlock(&mut) < 0 ){
			fflush(stdout);
			perror("failed");
			return -1;
		}
	}

	if ( pthread_mutex_unlock(&mut) == 0 ){
		printf("Should have failed\n");
		return -1;
	}

	if ( errno != EACCES ){
		printf("Should have faile with %d not %d\n", EACCES, errno);
		return -1;
	}
	errno = 0;

	printf("passed (%d Total Locks)\n", total_locks);

	printf("Test pthread_mutex_destroy()...");
	if ( pthread_mutex_destroy(&mut) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Stress Test pthread_mutex_destroy()...");
	if ( pthread_mutex_destroy(&mut) == 0 ){
		printf("Should have failed with EINVAL\n");
		return -1;
	}
	if ( errno != EINVAL ){
		printf("Should have failed with EINVAL not %d\n", errno);
		return -1;
	}
	errno = 0;
	printf("passed\n");
	return 0;
}



void * t0(void * args){
	printf("t0 lock...");
	fflush(stdout);

	if ( pthread_mutex_lock(&mutex_var) < 0 ){
		perror("t0 failed to lock");
		return NULL;
	}

	usleep(200*1000);

	printf("t0 unlock...");
	fflush(stdout);

	if ( pthread_mutex_unlock(&mutex_var) < 0 ){
		perror("t0 failed to unlock");
		return NULL;
	}

	return NULL;
}


