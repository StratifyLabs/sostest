/* Copyright 2011; All Rights Reserved
 * Please see http://www.coactionos.com/license.html for
 * licensing information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <reent.h>


#include "tests.h"


#define ATTR_TEST 1
#define CONDATTR_TEST 1
#define MUTEXATTR_TEST 1
#define RWLOCKATTR_TEST 1
#define PTHREAD_TEST 1
#define COND_TEST 1
#define MUTEX_TEST 1

int pthread_master_test(void){

	if ( ATTR_TEST ){
		if ( pthread_attr_test() < 0 ){
			printf("Attribute test failed\n");
			return -1;
		}
	}

	if ( CONDATTR_TEST ){
		if ( pthread_condattr_test() < 0 ){
			printf("Condition Attribute test failed\n");
			return -1;
		}
	}

	if ( MUTEXATTR_TEST ){
		if ( pthread_mutexattr_test() < 0 ){
			printf("Mutex Attribute test failed\n");
			return -1;
		}
	}

	if ( PTHREAD_TEST ){
		if ( pthread_test() < 0 ){
			printf("Pthread test failed\n");
			return -1;
		}
	}

	if ( COND_TEST ){
		if ( pthread_cond_test() < 0 ){
			printf("Cond test failed\n");
			return -1;
		}
	}

	if ( MUTEX_TEST ){
		if ( pthread_mutex_test() < 0 ){
			printf("Mutex test failed\n");
			return -1;
		}
	}

	return 0;
}

int create_thread(void * (*func)(void*), int stacksize, int detachstate){
	pthread_t t;
	pthread_attr_t pattr;

	if ( pthread_attr_init(&pattr) < 0 ){
		fflush(stdout);
		perror("attr_init failed");
		return -1;
	}

	if ( pthread_attr_setdetachstate(&pattr, detachstate) < 0 ){
		fflush(stdout);
		perror("setdetachstate failed");
		return -1;
	}

	if ( pthread_attr_setstacksize(&pattr, stacksize) < 0 ){
		fflush(stdout);
		perror("setstacksize failed");
		return -1;
	}

	if ( pthread_create(&t, &pattr, func, NULL) ){
		fflush(stdout);
		perror("create failed");
		return -1;
	}
	return t;
}


