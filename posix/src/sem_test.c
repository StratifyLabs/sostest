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


#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "tests.h"

#define NAMED_TEST 1
#define UNNAMED_TEST 1

static int named_test(void);
static int unnamed_test(void);

static void * t0(void * args);
static void * t1(void * args);

static sem_t unnamed;
#define SEM_NAME "my_sem"

int sem_test(void){

	if ( NAMED_TEST ){
		if ( named_test() ){
			return -1;
		}
	}

	if ( UNNAMED_TEST ){
		if ( unnamed_test() ){
			return -1;
		}
	}

	return 0;
}


int named_test(void){
	sem_t * sem;
	int value;
	int tmp;

	errno = 0;
	printf("Test sem_open()...");
	fflush(stdout);
	sem = sem_open(SEM_NAME, O_CREAT|O_EXCL|O_RDWR, 0666, 4);
	if ( sem == SEM_FAILED ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");


	//lock up the semaphore
	printf("Test sem_trywait()...");
	value = 0;
	errno = 0;
	while( sem_trywait(sem) == 0 ){
		printf("got sem...");
		value++;
	}

	if ( errno != EAGAIN ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	errno = 0;

	if ( value != 4 ){
		sem_getvalue(sem, &tmp);
		printf("Test failed (sem value is %d ?= %d)\n", tmp, value);
		return -1;
	}
	printf("passed\n");

	printf("Test sem_wait()...");
	fflush(stdout);

	if( create_thread(t0, 2048, PTHREAD_CREATE_DETACHED) < 0 ){
		printf("Failed to create the thread (%d)\n", errno);
		return -1;
	}

	usleep(100*1000);

	while(value--){
		printf("post sem...");
		fflush(stdout);
		if ( sem_post(sem) < 0 ){
			printf("Failed to post sem (%d)\n", errno);
		}
		usleep(1000);
	}
	printf("passed\n");



	printf("Test sem_close()...");
	if ( sem_close(sem) ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test sem_unlink()...");
	if ( sem_unlink(SEM_NAME) ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");


	return 0;
}

int unnamed_test(void){
	int tmp;

	sem_t * sem = NULL;
	int value;

	printf("Test sem_init()...");
	if ( sem_init(&unnamed, 0, 4) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	//lock up the semaphore
	printf("Test sem_trywait()...");
	errno = 0;
	value = 0;
	while( sem_trywait(&unnamed) == 0 ){
		printf("got lock...");
		value++;
	}
	if ( errno != EAGAIN ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	errno = 0;
	if ( value != 4 ){
		sem_getvalue(sem, &tmp);
		printf("Test failed (sem value is %d ?= %d)\n", tmp, value);
		return -1;
	}
	printf("passed\n");

	printf("Test sem_wait()...");
	fflush(stdout);

	if( create_thread(t1, 2048, PTHREAD_CREATE_DETACHED) < 0 ){
		printf("Failed to create the thread (%d)\n", errno);
		return -1;
	}

	usleep(500*1000);

	while(value--){
		printf("post...");
		fflush(stdout);
		if ( sem_post(&unnamed) < 0 ){
			fflush(stdout);
			perror("sem_post() failed");
			return -1;
		}
		usleep(1000);
	}

	printf("passed\n");



	return 0;
}

void * t0(void * args){
	sem_t * sem;

	sem = sem_open(SEM_NAME, 0);
	if ( sem == SEM_FAILED ){
		perror("t0 sem_open() failed");
		return NULL;
	}

	printf("t0 wait...");
	fflush(stdout);
	if ( sem_wait(sem) < 0 ){
		perror("t0 sem_wait() failed");
		return NULL;
	}
	printf("t0 got sem...");
	fflush(stdout);

	if ( sem_close(sem) < 0 ){
		perror("t0 sem_close() failed");
	}

	return NULL;
}

void * t1(void * args){

	printf("t1 wait...");
	fflush(stdout);

	if ( sem_wait(&unnamed) < 0 ){
		printf("Failed to wait for sem (%d)\n", errno);
	}

	printf("t1 done...");
	fflush(stdout);
	return NULL;
}
