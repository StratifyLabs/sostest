/* Copyright 2011; All Rights Reserved
 * Please see http://www.coactionos.com/license.html for
 * licensing information.
 */

#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h>

#include "tests.h"

#define DETACHSTATE_TOTAL 10
test_case_t detachstates[DETACHSTATE_TOTAL] = {
		{ .case_number = PTHREAD_CREATE_DETACHED, .err_number = 0 },
		{ .case_number = PTHREAD_CREATE_JOINABLE, .err_number = 0 },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = 100, .err_number = EINVAL },
		{ .case_number = 1000, .err_number = EINVAL },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = -500, .err_number = EINVAL },
		{ .case_number = -2000, .err_number = EINVAL },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100000, .err_number = EINVAL },
};

#define INHERITSCHED_TOTAL 10
test_case_t inheritscheds[INHERITSCHED_TOTAL] = {
		{ .case_number = PTHREAD_INHERIT_SCHED, .err_number = 0 },
		{ .case_number = PTHREAD_EXPLICIT_SCHED, .err_number = 0 },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = 100, .err_number = EINVAL },
		{ .case_number = 1000, .err_number = EINVAL },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = -500, .err_number = EINVAL },
		{ .case_number = -2000, .err_number = EINVAL },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100000, .err_number = EINVAL },
};

#define SCHEDPOLICY_TOTAL 10
test_case_t schedpolicies[SCHEDPOLICY_TOTAL] = {
		{ .case_number = SCHED_FIFO, .err_number = 0 },
		{ .case_number = SCHED_RR, .err_number = 0 },
		{ .case_number = SCHED_OTHER, .err_number = 0 },
		{ .case_number = 100, .err_number = EINVAL },
		{ .case_number = 1000, .err_number = EINVAL },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = -500, .err_number = EINVAL },
		{ .case_number = -2000, .err_number = EINVAL },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100000, .err_number = EINVAL },
};

#define SCOPE_TOTAL 10
test_case_t scopes[SCOPE_TOTAL] = {
		{ .case_number = PTHREAD_SCOPE_PROCESS, .err_number = EINVAL },
		{ .case_number = PTHREAD_SCOPE_SYSTEM, .err_number = 0 },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100, .err_number = EINVAL },
		{ .case_number = 1000, .err_number = EINVAL },
		{ .case_number = -1, .err_number = EINVAL },
		{ .case_number = -500, .err_number = EINVAL },
		{ .case_number = -2000, .err_number = EINVAL },
		{ .case_number = 10000, .err_number = EINVAL },
		{ .case_number = 100000, .err_number = EINVAL },
};



static int test_schedparam(pthread_attr_t * attr, int prio, int policy);

int pthread_attr_test(void){
	pthread_attr_t attr;
	int tmp;
	void * stackaddr;
	int i;

	printf("Test pthread_attr_init()...");
	if ( pthread_attr_init(&attr) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");


	if ( set_get_test("pthread_attr_set/getdetachstate()",
			(int (*)(void*,int))pthread_attr_setdetachstate,
			(int (*)(void*,int*))pthread_attr_getdetachstate,
			&attr,
			detachstates,
			DETACHSTATE_TOTAL,
			0) ){
		return -1;
	}

	printf("Test pthread_attr_getguardsize()...");
	if ( pthread_attr_getguardsize(&attr, (size_t*)&tmp) < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	if ( set_get_test("pthread_attr_set/getinheritsched()",
			(int (*)(void*,int))pthread_attr_setinheritsched,
			(int (*)(void*,int*))pthread_attr_getinheritsched,
			&attr,
			inheritscheds,
			INHERITSCHED_TOTAL,
			0) ){
		return -1;
	}

	if ( set_get_test("pthread_attr_set/getschedpolicy()",
			(int (*)(void*,int))pthread_attr_setschedpolicy,
			(int (*)(void*,int*))pthread_attr_getschedpolicy,
			&attr,
			schedpolicies,
			SCHEDPOLICY_TOTAL,
			0) ){
		return -1;
	}


	printf("Test pthread_attr_set/getschedparam() with SCHED_FIFO...");
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	for(i=sched_get_priority_min(SCHED_FIFO); i <= sched_get_priority_max(SCHED_FIFO); i++){
		if ( test_schedparam(&attr, i, 0) < 0 ){
			return -1;
		}
	}

	if ( test_schedparam(&attr, sched_get_priority_max(SCHED_FIFO) + 1, EINVAL) < 0 ){
		return -1;
	}

	if ( test_schedparam(&attr, sched_get_priority_min(SCHED_FIFO) - 1, EINVAL) < 0 ){
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_attr_set/getschedparam() with SCHED_RR...");
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	for(i=sched_get_priority_min(SCHED_RR); i <= sched_get_priority_max(SCHED_RR); i++){
		if ( test_schedparam(&attr, i, 0) < 0 ){
			return -1;
		}
	}

	if ( test_schedparam(&attr, sched_get_priority_max(SCHED_RR) + 1, EINVAL) < 0 ){
		return -1;
	}

	if ( test_schedparam(&attr, sched_get_priority_min(SCHED_RR) - 1, EINVAL) < 0 ){
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_attr_set/getschedparam() with SCHED_OTHER...");
	pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
	for(i=sched_get_priority_min(SCHED_OTHER); i <= sched_get_priority_max(SCHED_OTHER); i++){
		if ( test_schedparam(&attr, i, 0) < 0 ){
			return -1;
		}
	}

	if ( test_schedparam(&attr, sched_get_priority_max(SCHED_OTHER) + 1, EINVAL) < 0 ){
		return -1;
	}

	if ( test_schedparam(&attr, sched_get_priority_min(SCHED_OTHER) - 1, EINVAL) < 0 ){
		return -1;
	}
	printf("passed\n");

	if ( set_get_test("pthread_attr_set/getscope()",
			(int (*)(void*,int))pthread_attr_setscope,
			(int (*)(void*,int*))pthread_attr_getscope,
			&attr,
			scopes,
			SCOPE_TOTAL,
			0) ){
		return -1;
	}

	printf("Test pthread_attr_getstackaddr()...");
	if ( pthread_attr_getstackaddr(&attr, &stackaddr) ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test pthread_attr_destroy()...");
	if ( pthread_attr_destroy(&attr) ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Stress Test pthread_attr_destroy()...");
	if ( pthread_attr_destroy(&attr) == 0){
		printf("should have failed\n");
		return -1;
	}
	if ( errno != EINVAL ){
		printf("errno should be %d not %d\n", EINVAL, errno);
		return -1;
	}
	printf("passed\n");
	return 0;
}

int test_schedparam(pthread_attr_t * attr, int prio, int err_number){
	struct sched_param param;
	param.sched_priority = prio;

	errno = 0;
	pthread_attr_setschedparam(attr, &param);
	if ( err_number != errno ){
		if ( errno == 0 ){
			printf("expected errno %d\n", errno);
		} else {
			printf("prio %d ", prio);
			fflush(stdout);
			perror("failed to set");
		}
		return -1;
	}

	if ( errno == 0 ){
		if ( pthread_attr_getschedparam(attr, &param) < 0 ){
			fflush(stdout);
			perror("failed to get");
			return -1;
		}

		if ( param.sched_priority != prio ){
			printf("failed to set/get (%d != %d)\n", param.sched_priority, prio);
			return -1;
		}
	}
	return 0;
}
