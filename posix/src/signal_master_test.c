
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <sched.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stratify/stratify.h>
#include "tests.h"

#define SLAVE_APP "/home/sigslave"

int signal_master_test(void){
	pid_t pid;
	int err;
	int stat;
	char exec_path[PATH_MAX];

	if ( sigset_test() < 0 ){
		printf("sigset test failed\n");
		return -1;
	}

	if ( signal_test() < 0 ){
		printf("signal test failed\n");
		return -1;
	}

	if( access(SLAVE_APP, F_OK) < 0 ){
		printf("Test SIGKILL (process)...failed (slave application not found)\n");
		return -1;
	}

	printf("Test SIGKILL (process)...");
	fflush(stdout);
	usleep(5000);

	errno = 0;
	if( (pid = launch(SLAVE_APP, exec_path, 0, 0, 0, 0, 0)) < 0 ){
		perror("failed to launch sigslave");
		return -1;
	}

	sleep(3);

	printf("Send kill to %d...",  pid);
	fflush(stdout);
	err = kill(pid, SIGKILL);

	stat = 0;
	wait(&stat);

	if( unlink(exec_path) < 0 ){
		perror("failed to cleanup SLAVE_APP");
		return -1;
	}

	if ( stat == SIGKILL ){
		printf("passed\n");
	} else {
		printf("failed: bad exit status (%d)\n", err);
	}

	printf("All Tests Completed successfully\n");

	return 0;
}


