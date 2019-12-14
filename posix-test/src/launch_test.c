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

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sos/sos.h>
#include <sos/dev/appfs.h>
#include <sys/wait.h>

#include "tests.h"

#define LAUNCH_SLAVE_PATH "/home/launchslave"

int update_progress(const void * context, int progress, int max){
	printf(".");
	return 0;
}


int launch_test(){
	pid_t pid;
	pid_t pid_alt;
	int status;
	char exec_path[PATH_MAX];
	char exec_path_alt[PATH_MAX];
	int ret;
	int fd;
	char buffer[64];

	printf("Test launch() (BAD PATH)...");
	fflush(stdout);
	if( launch("1",
				  0,
				  0,
				  0,
				  0,
				  update_progress,
				  0,
				  0) < 0 ){
		if ( errno == ENOENT ){
			printf("passed\n");
		} else {
			printf("failed with %d not %d\n", errno, ENOENT);
		}
	} else {
		printf("failed (returned success on empty path)\n");
	}

	printf("Test launch() (INVALID PATH)...");
	fflush(stdout);
	if( launch("/home/NO_PROGRAM", 0, 0, 0, 0, update_progress, 0, 0) < 0 ){
		if ( errno == ENOENT ){
			printf("passed\n");
		} else {
			printf("failed with %d not %d\n", errno, ENOENT);
		}
	} else {
		printf("failed (returned success on empty path)\n");
	}


	printf("Test launch() (no children)...");
	fflush(stdout);
	errno = 0;
	wait(&status);
	if( errno != ECHILD ){
		printf("failed (%d != %d)\n", errno, ECHILD);
	} else {
		printf("passed\n");
	}


	printf("Test launch()...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, 0, 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {
		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}
	}

	printf("Test launch() (relaunch)...");
	fflush(stdout);
	if( (pid = launch("/app/ram/launchslave", 0, 0, 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {
		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);
	}

	printf("Test launch() (with options)...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, "2 3 4 5 6 7", 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {

		do {
			errno = 0;
			ret = wait(&status);
		} while( (ret < 0) && (errno = EINTR) );

		unlink(exec_path);

		if( ((status >> 8) & 0xff) != 7 ){
			printf("Failed (return value %d != %d)\n", 7, (status >> 8) & 0xff);
			return -1;
		}

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}
	}

	printf("Test launch() (orphan)...");
	unlink("/home/orphan.txt");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, "orphan", APPFS_FLAG_IS_ORPHAN, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {

		//orphan creates a file called orphan.txt
		usleep(500*1000);

		fd = open("/home/orphan.txt", O_RDONLY);
		if( fd < 0 ){
			printf("orphan failed to create file\n");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);

	}

	printf("Test launch() (flash)...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, 0, APPFS_FLAG_IS_FLASH, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {
		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);
	}

	printf("Test launch() (child wait)...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, "wait", 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {
		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);
	}

	printf("Test launch() (parent wait)...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, 0, 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {

		usleep(100*1000);

		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);
	}

	printf("Test launch() (SIGCHLD)...");
	fflush(stdout);
	sprintf(buffer, "child %d", getpid());
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, buffer, 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {

		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);
	}

	printf("Test launch() (SIGCHLD wait)...");
	fflush(stdout);
	sprintf(buffer, "child %d", getpid());
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, buffer, 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	} else {

		usleep(50*1000);

		do {
			errno = 0;
			ret = wait(&status);
		} while( errno == EINTR );

		if( errno != 0 ){
			perror("failed to wait");
		} else {
			printf("passed\n");
		}

		unlink(exec_path);
	}

	printf("Test launch() (two copies)...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, "1", 0, 0, update_progress, 0, 0)) < 0 ){
		perror("failed to launch");
		return -1;
	}

	usleep(50*1000);

	if( (pid_alt = launch(LAUNCH_SLAVE_PATH, exec_path_alt, "2", 0, 0, update_progress, 0, 0)) < 0 ){
		wait(&status);
		unlink(exec_path);
		perror("failed to launch alt");
		return -1;
	}

	//wait for both to finish
	fd = 0;
	do {
		ret = wait(&status);
		if( ret == pid ){
			fd++;
		}
		if( ret == pid_alt ){
			fd++;
		}
	} while( ret > 0 );

	if( errno != ECHILD ){
		printf("failed -- errno %d != %d\n", ECHILD, errno);
	}

	unlink(exec_path);
	unlink(exec_path_alt);

	if( fd == 2 ){
		printf("passed\n");
	} else {
		printf("failed (didn't wait for 2 processes)\n");
	}

	printf("Test launch() (EXCESS RAM)...");
	fflush(stdout);
	if( (pid = launch(LAUNCH_SLAVE_PATH, exec_path, 0, 0, 128*1024, update_progress, 0, 0)) < 0 ){
		if( errno == ENOSPC ){
			printf("passed\n");
		} else {
			printf("failed -- errno %d != %d\n", errno, ENOSPC);
		}
	} else {
		printf("failed -- not enough RAM");
	}



	return 0;
}
