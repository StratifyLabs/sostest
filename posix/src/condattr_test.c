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


#define PSHARED_TOTAL 10
test_case_t pshareds[PSHARED_TOTAL] = {
		{ .case_number = 0, .err_number = 0 },
		{ .case_number = 1, .err_number = 0 },
		{ .case_number = 2, .err_number = 0 },
		{ .case_number = 100, .err_number = 0 },
		{ .case_number = 1000, .err_number = 0 },
		{ .case_number = -1, .err_number = 0 },
		{ .case_number = -500, .err_number = 0 },
		{ .case_number = -2000, .err_number = 0 },
		{ .case_number = 10000, .err_number = 0 },
		{ .case_number = 100000, .err_number = 0 }
};

#define CLOCK_TOTAL 10
test_case_t clock_cases[CLOCK_TOTAL] = {
		{ .case_number = CLOCK_REALTIME, .err_number = ENOTSUP },
		{ .case_number = CLOCK_MONOTONIC, .err_number = ENOTSUP },
		{ .case_number = 0, .err_number = ENOTSUP },
		{ .case_number = 100, .err_number = ENOTSUP },
		{ .case_number = 1000, .err_number = ENOTSUP },
		{ .case_number = -1, .err_number = ENOTSUP },
		{ .case_number = -500, .err_number = ENOTSUP },
		{ .case_number = -2000, .err_number = ENOTSUP },
		{ .case_number = 10000, .err_number = ENOTSUP },
		{ .case_number = 100000, .err_number = ENOTSUP }
};

int pthread_condattr_test(){
	pthread_condattr_t attr;
	clockid_t id;

	printf("Test pthread_condattr_init()...");
	if ( pthread_condattr_init(&attr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");


	if ( set_get_test("pthread_condattr_set/getpshared()",
			(int (*)(void*,int))pthread_condattr_setpshared,
			(int (*)(void*,int*))pthread_condattr_getpshared,
			&attr,
			pshareds,
			PSHARED_TOTAL,
			0) ){
		return -1;
	}

	if ( set_get_test("pthread_condattr_set/getclock()",
			(int (*)(void*,int))pthread_condattr_setclock,
			(int (*)(void*,int*))pthread_condattr_getclock,
			&attr,
			clock_cases,
			CLOCK_TOTAL,
			0) ){
		return -1;
	}

	printf("Test pthread_condattr_getclock()...");
	if ( pthread_condattr_getclock(&attr, &id) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( id != CLOCK_REALTIME ){
		printf("failed to get\n");
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_condattr_destroy()...");
	if ( pthread_condattr_destroy(&attr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Stress Test pthread_condattr_destroy()...");
	if ( pthread_condattr_destroy(&attr) == 0 ){
		printf("failed\n");
		return -1;
	}
	if ( errno == EINVAL ){
		errno = 0;
		printf("passed\n");
	} else {
		printf("failed errno is %d not %d\n", errno, EINVAL);
	}

	return 0;

}
