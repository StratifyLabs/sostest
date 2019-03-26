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
#include <sos/sos.h>
#include "tests.h"

#define SLAVE_APP "/app/flash/sigslave"

int signal_master_test(){
	pid_t pid;
	int err;
	int stat;
	int i;
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
	if( (pid = launch(SLAVE_APP, exec_path, 0, 0, 0, 0, 0, 0)) < 0 ){
		perror("failed to launch sigslave");
		return -1;
	}

	printf("launched %d...", pid);
	fflush(stdout);

	usleep(100*1000);


	for(i=0; i < 50; i++){
		printf("Send SIGUSR1 %d...",  i);
		fflush(stdout);
		errno = 0;
		err = kill(pid, SIGUSR1);
		if( err < 0 ){
			printf("Failed to send signal (%d)\n", errno);
			perror("kill SIGUSR1 failed");
			break;
		}
		if( i % 4 == 0 ){
			usleep(50*1000);
		}
	}


	printf("Send SIGINT to %d...",  pid);
	fflush(stdout);
	err = kill(pid, SIGINT);

	stat = 0;
	wait(&stat);

	/*
	if( unlink(exec_path) < 0 ){
		perror("failed to cleanup SLAVE_APP");
		return -1;
	}
	*/

	if ( stat == SIGINT ){
		printf("passed\n");
	} else {
		printf("failed: bad exit status (%d)\n", err);
	}

	printf("All Tests Completed successfully\n");

	return 0;
}


