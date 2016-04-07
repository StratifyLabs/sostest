
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static int test_min_max(int (*func)(int), const char * func_name);
static int test_schedparam(void);
static int test_scheduler(void);
static int test_rr_interval(void);

int sched_test(void){

	if ( test_min_max(sched_get_priority_max, "sched_get_priority_max") < 0 ){
		printf("sched_get_priority_max() test failed\n");
		return -1;
	}

	if ( test_min_max(sched_get_priority_min, "sched_get_priority_min") < 0 ){
		printf("sched_get_priority_max() test failed\n");
		return -1;
	}


	if ( test_schedparam() < 0 ){
		printf("sched_get_priority_max() test failed\n");
		return -1;
	}

	if ( test_scheduler() < 0 ){
		printf("sched_get_priority_max() test failed\n");
		return -1;
	}

	if ( sched_yield() < 0 ){
		perror("failed to yield");
		return -1;
	}

	if ( test_rr_interval() < 0 ){
		printf("sched_get_priority_max() test failed\n");
		return -1;
	}


	return 0;
}

int test_rr_interval(void){
	struct timespec ts;
	int err;

	printf("Test sched_rr_get_interval()...");
	memset(&ts, 0, sizeof(struct timespec));
	err = sched_rr_get_interval(getpid(), &ts);
	if ( err == 0 ){
		if ( ts.tv_nsec == 0 ){
			printf("Failed:  RR time is zero\n");
			return -1;
		} else {
			printf("%ld ns...", ts.tv_nsec);
		}
	} else {
		fflush(stdout);
		perror("failed");
		return -1;
	}

	printf("passed\n");

	printf("Test sched_rr_get_interval() (ERSCH)...");
	memset(&ts, 0, sizeof(struct timespec));
	err = sched_rr_get_interval(-1, &ts);
	if ( (err == 0) || (errno != ESRCH) ){
		fflush(stdout);
		perror("should have failed with ESRCH");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	return 0;
}

int test_min_max(int (*func)(int), const char * func_name){
	int prio;
	printf("Test %s() (SCHED_OTHER)...", func_name);
	prio = func(SCHED_OTHER);
	if ( prio >= 0 ){
		printf("passed\n");
	} else {
		fflush(stdout);
		perror("failed");
		return -1;
	}

	printf("Test %s() (SCHED_FIFO)...", func_name);
	prio = func(SCHED_FIFO);
	if ( prio >= 0 ){
		printf("%d...passed\n", prio);
	} else {
		fflush(stdout);
		perror("failed");
		return -1;
	}

	printf("Test %s() (SCHED_RR)...", func_name);
	prio = func(SCHED_RR);
	if ( prio >= 0 ){
		printf("%d...passed\n", prio);
	} else {
		fflush(stdout);
		perror("failed");
		return -1;
	}

	printf("Test %s() (EINVAL)...", func_name);
	prio = func(100);
	if ( prio >= 0 ){
		printf("failed (should have returned -1 not %d)\n", prio);
	} else {
		if ( errno == EINVAL ){
			errno = 0;
			printf("passed\n");
		} else {
			fflush(stdout);
			perror("Should have faile with EINVAL");
			return -1;
		}
	}
	return 0;
}

int test_scheduler(void){
	int policy;
	int err;
	struct sched_param param;
	int i;
	int max_prio;
	int min_prio;

	printf("Test sched_setscheduler() (SCHED_FIFO)...");
	min_prio = sched_get_priority_min(SCHED_FIFO);
	max_prio = sched_get_priority_max(SCHED_FIFO);
	for(i = min_prio; i <= max_prio; i++){
		param.sched_priority = i;
		if ( sched_setscheduler(getpid(), SCHED_FIFO, &param) < 0 ){
			fflush(stdout);
			perror("failed");
			return -1;
		}
	}

	printf("passed\n");

	printf("Test sched_getscheduler() (SCHED_FIFO)...");
	policy = sched_getscheduler(getpid());
	if ( policy < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( policy == SCHED_FIFO ){
		printf("passed\n");
	} else {
		printf("failed policy should be SCHED_FIFO not %d\n", policy);
		return -1;
	}

	printf("Test sched_setscheduler() (SCHED_RR)...");
	min_prio = sched_get_priority_min(SCHED_RR);
	max_prio = sched_get_priority_max(SCHED_RR);
	for(i = min_prio; i <= max_prio; i++){
		param.sched_priority = i;
		if ( sched_setscheduler(getpid(), SCHED_RR, &param) < 0 ){
			fflush(stdout);
			perror("failed");
			return -1;
		}
	}

	printf("passed\n");

	printf("Test sched_getscheduler() (SCHED_RR)...");
	policy = sched_getscheduler(getpid());
	if ( policy < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( policy == SCHED_RR ){
		printf("passed\n");
	} else {
		printf("failed policy should be SCHED_RR not %d\n", policy);
		return -1;
	}

	printf("Test sched_setscheduler() (SCHED_OTHER)...");
	min_prio = sched_get_priority_min(SCHED_OTHER);
	max_prio = sched_get_priority_max(SCHED_OTHER);
	for(i = min_prio; i <= max_prio; i++){
		param.sched_priority = i;
		if ( sched_setscheduler(getpid(), SCHED_OTHER, &param) < 0 ){
			fflush(stdout);
			perror("failed");
			return -1;
		}
	}

	printf("passed\n");

	printf("Test sched_getscheduler() (SCHED_OTHER)...");
	policy = sched_getscheduler(getpid());
	if ( policy < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( policy == SCHED_OTHER ){
		printf("passed\n");
	} else {
		printf("failed policy should be SCHED_OTHER not %d\n", policy);
		return -1;
	}

	printf("Test sched_setscheduler() (ESRCH)...");
	err = sched_setscheduler(-1, SCHED_OTHER, &param);
	if ( (err == 0) || (errno != ESRCH) ){
		fflush(stdout);
		perror("should have failed with ESRCH");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_setscheduler() (EINVAL:policy)...");
	err = sched_setscheduler(getpid(), 500, &param);
	if ( (err == 0) || (errno != EINVAL) ){
		fflush(stdout);
		perror("should have failed with EINVAL");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_setscheduler() (EINVAL:priority)...");
	param.sched_priority = sched_get_priority_max(SCHED_FIFO) + 1;
	err = sched_setscheduler(getpid(), SCHED_FIFO, &param);
	if ( (err == 0) || (errno != EINVAL) ){
		fflush(stdout);
		perror("should have failed with EINVAL");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_setscheduler() (EINVAL:priority)...");
	param.sched_priority = sched_get_priority_max(SCHED_RR) + 1;
	err = sched_setscheduler(getpid(), SCHED_RR, &param);
	if ( (err == 0) || (errno != EINVAL) ){
		fflush(stdout);
		perror("should have failed with EINVAL");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_setscheduler() (EINVAL:priority)...");
	param.sched_priority = sched_get_priority_max(SCHED_OTHER) + 1;
	err = sched_setscheduler(getpid(), SCHED_OTHER, &param);
	if ( (err == 0) || (errno != EINVAL) ){
		fflush(stdout);
		perror("should have failed with EINVAL");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_getscheduler() (ESRCH)...");
	err = sched_getscheduler(-1);
	if ( (err == 0) || (errno != ESRCH) ){
		fflush(stdout);
		perror("should have failed with ESRCH");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	return 0;
}

int test_schedparam(void){
	int err;
	struct sched_param param;

	printf("Test sched_getparam()...");
	err = sched_getparam(getpid(), &param);
	if ( err < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( param.sched_priority == 0 ){
		printf("passed\n");
	} else {
		printf("unexpected sched priority\n");
	}

	printf("Test sched_getparam() (ESRCH)...");
	err = sched_getparam(-1, &param);
	if ( (err == 0) || (errno != ESRCH) ){
		fflush(stdout);
		perror("should have failed with ESRCH");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_setparam()...");
	param.sched_priority = 0;
	err = sched_setparam(getpid(), &param);
	if ( err < 0 ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	if ( param.sched_priority == 0 ){
		printf("passed\n");
	} else {
		printf("unexpected sched priority\n");
	}

	printf("Test sched_setparam() (EINVAL)...");
	param.sched_priority = 100;
	err = sched_setparam(getpid(), &param);
	if ( (err == 0) || (errno != EINVAL) ){
		fflush(stdout);
		perror("should have failed with EINVAL");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}

	printf("Test sched_setparam() (ESRCH)...");
	err = sched_setparam(-1, &param);
	if ( (err == 0) || (errno != ESRCH) ){
		fflush(stdout);
		perror("should have failed with ESRCH");
		return -1;
	} else {
		errno = 0;
		printf("passed\n");
	}


	return 0;
}

