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
static void * t1(void * args);


static volatile int t0_status;
static volatile int t1_status;
pthread_cond_t cond_var;
pthread_mutex_t mutex_var;

static int prep_test(void);


int pthread_cond_test(void){
	pthread_condattr_t attr;
	pthread_mutexattr_t mattr;

	if ( pthread_mutexattr_init(&mattr) < 0 ){
		perror("pthread_mutexattr_init() failed");
		return -1;
	}

	if ( pthread_mutex_init(&mutex_var, &mattr) < 0 ){
		perror("pthread_mutex_init() failed");
		return -1;
	}

	if ( pthread_condattr_init(&attr) < 0 ){
		perror("pthread_condattr_init() failed");
		return -1;
	}

	printf("Test pthread_cond_init()...");
	if ( pthread_cond_init(&cond_var, &attr) ){
		fflush(stdout);
		perror("pthread_cond_init() failed");
		return -1;
	}
	printf("passed\n");


	if ( prep_test() < 0 ){
		return -1;
	}

	printf("Test pthread_cond_signal()...");
	fflush(stdout);
	if ( pthread_cond_signal(&cond_var) < 0 ){
		fflush(stdout);
		perror("pthread_cond_signal() failed");
		return -1;
	}

	while( (t0_status == 0) && (t1_status == 0) ){
		usleep(100);
	}

	if ( pthread_cond_signal(&cond_var) < 0 ){
		fflush(stdout);
		perror("pthread_cond_signal() failed");
		return -1;
	}


	while( (t0_status == 0) || (t1_status == 0) ){
		usleep(100);
	}

	if ( (t0_status != 1) || (t1_status != 1) ){
		printf("Failed t0/t1 didn't wait for condition %d %d\n", t0_status, t1_status);
		return -1;
	}
	printf("passed\n");

	if ( prep_test() < 0 ){
		return -1;
	}

	printf("Test pthread_cond_broadcast()...");
	fflush(stdout);
	if ( pthread_cond_broadcast(&cond_var) < 0 ){
		fflush(stdout);
		perror("pthread_cond_broadcast() failed");
		return -1;
	}

	while( (t0_status == 0) || (t1_status == 0) ){
		usleep(100);
	}

	if ( (t0_status != 1) || (t1_status != 1) ){
		printf("Failed t0/t1 didn't wait for condition %d %d\n", t0_status, t1_status);
		return -1;
	}
	printf("passed\n");

	return 0;
}

void * t0(void * args){
	//lock the mutex
	if ( pthread_mutex_lock(&mutex_var) < 0 ){
		t0_status = -2;
		return NULL;
	}
	t0_status = 0;
	//block on the condition
	if ( pthread_cond_wait(&cond_var, &mutex_var) < 0 ){
		perror("t0 pthread_cond_wait() failed");
		return NULL;
	}
	printf("t0...");
	fflush(stdout);
	t0_status = 1;
	return NULL;
}


void * t1(void * args){
	//lock the mutex
	if ( pthread_mutex_lock(&mutex_var) < 0 ){
		t1_status = -2;
		return NULL;
	}

	t1_status = 0;
	//block on the condition
	pthread_cond_wait(&cond_var, &mutex_var);
	printf("t1...");
	fflush(stdout);
	t1_status = 1;
	return NULL;
}

int prep_test(void){
	t0_status = -1;
	t1_status = -1;

	if ( create_thread(t0, 2048, PTHREAD_CREATE_DETACHED) < 0 ){
		return -1;
	}

	if ( create_thread(t1, 2048, PTHREAD_CREATE_DETACHED) < 0 ){
		return -1;
	}

	while(t0_status == -1 ){
		usleep(100);
	}

	while(t1_status == -1 ){
		usleep(100);
	}

	if ( t0_status != 0 ){
		printf("Failed t0 didn't wait for condition %d\n", t0_status);
		return -1;
	}

	if ( t1_status != 0 ){
		printf("Failed t1 didn't wait for condition %d\n", t1_status);
		return -1;
	}
	return 0;
}

