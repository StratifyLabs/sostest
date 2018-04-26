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

#define PROTOCOLS_TOTAL 10
test_case_t protocols[PROTOCOLS_TOTAL] = {
		{ .case_number = PTHREAD_PRIO_NONE, .err_number = 0 },
		{ .case_number = PTHREAD_PRIO_INHERIT, .err_number = 0 },
		{ .case_number = PTHREAD_PRIO_PROTECT, .err_number = 0 },
		{ .case_number = 100, .err_number = EINVAL },
		{ .case_number = 1000, .err_number = EINVAL },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = -500, .err_number = EINVAL },
		{ .case_number = -2000, .err_number = EINVAL },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100000, .err_number = EINVAL },
};

#define TYPES_TOTAL 10
test_case_t types[TYPES_TOTAL] = {
		{ .case_number = PTHREAD_MUTEX_NORMAL, .err_number = 0 },
		{ .case_number = PTHREAD_MUTEX_RECURSIVE, .err_number = 0 },
		{ .case_number = PTHREAD_MUTEX_ERRORCHECK, .err_number = EINVAL },
		{ .case_number = PTHREAD_MUTEX_DEFAULT, .err_number = EINVAL },
		{ .case_number = 1000, .err_number = EINVAL },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = -500, .err_number = EINVAL },
		{ .case_number = -2000, .err_number = EINVAL },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100000, .err_number = EINVAL },
};


int pthread_mutexattr_test(){
	pthread_mutexattr_t attr;
	int tmp;
	int i;

	printf("Test pthread_mutexattr_init()...");
	if ( pthread_mutexattr_init(&attr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_mutexattr_set/getprioceiling()...");
	for(i = sched_get_priority_min(SCHED_OTHER); i <= sched_get_priority_max(SCHED_OTHER); i++ ){
		if ( pthread_mutexattr_setprioceiling(&attr, i) < 0 ){
			fflush(stdout);
			perror("failed");
			return -1;
		}

		if ( pthread_mutexattr_getprioceiling(&attr, &tmp) < 0 ){
			fflush(stdout);
			perror("failed");
			return -1;
		}

		if ( tmp != i ){
			printf("failed to set/get (%d != %d)\n", tmp, i);
			return -1;
		}
	}
	printf("passed\n");


	if ( set_get_test("pthread_mutexattr_set/getprotocol()",
			(int (*)(void*,int))pthread_mutexattr_setprotocol,
			(int (*)(void*,int*))pthread_mutexattr_getprotocol,
			&attr,
			protocols,
			PROTOCOLS_TOTAL,
			0) ){
		return -1;
	}

	printf("Test pthread_mutexattr_set/getpshared()...");
	tmp = 1;
	if ( pthread_mutexattr_setpshared(&attr, tmp) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( pthread_mutexattr_getpshared(&attr, &tmp) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( tmp != 1 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");


	if ( set_get_test("pthread_mutexattr_set/gettype()",
			(int (*)(void*,int))pthread_mutexattr_settype,
			(int (*)(void*,int*))pthread_mutexattr_gettype,
			&attr,
			types,
			TYPES_TOTAL,
			0) ){
		return -1;
	}

	printf("Test pthread_mutexattr_destroy()...");
	if ( pthread_mutexattr_destroy(&attr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Stress Test pthread_mutexattr_destroy()...");
	if ( pthread_mutexattr_destroy(&attr) == 0 ){
		printf("should have failed\n");
		return -1;
	}
	errno = 0;
	printf("passed\n");

	return 0;
}
