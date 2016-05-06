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


#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "tests.h"


static volatile int sig_sent;
static void signal_catching_function(int sig);

int signal_test(void){
	int sig;
	sigset_t set;
	sigset_t oset;
	for(sig = 0; sig < 32; sig++){
		printf("Test signal(%d)...", sig);
		if ( sig != SIGCHLD ){
			errno = 0;
			if ( signal(sig, signal_catching_function) == SIG_ERR ){
				if ( !((sig == SIGKILL) || (sig == SIGSTOP)) ){
					if ( errno != EINVAL ){
						fflush(stdout);
						perror("failed");
						return -1;
					}
					errno = 0;
				}
			}
		}
		printf("passed\n");
	}


	for(sig = 0; sig < 32; sig++){
		printf("Test sigprocmask (%d)...", sig);
		if ( sig != SIGCHLD ){
			errno = 0;

			sigemptyset(&set);
			sigaddset(&set, sig);
			sig_sent = -1;
			if( sigprocmask(SIG_BLOCK, &set, 0) <  0){
				fflush(stdout);
				perror("sigprocmask failed");
				return -1;
			}

			if ( signal(sig, signal_catching_function) == SIG_ERR ){
				if ( !((sig == SIGKILL) || (sig == SIGSTOP)) ){
					if ( errno != EINVAL ){
						fflush(stdout);
						perror("failed");
						return -1;
					}
					errno = 0;
				}
			}

			if( sig_sent != -1 ){
				fflush(stdout);
				printf("failed (handler was executed)\n");
				return -1;
			}

		}
		printf("passed\n");
	}

	printf("Test sigprocmask EINVAL...");
	errno = 0;
	sigprocmask(200, &set, 0);
	if( errno != EINVAL ){
		printf("failed (errno is not EINVAL)\n");
		return -1;
	}
	printf("passed\n");


	sigfillset(&set);
	sigprocmask(SIG_UNBLOCK, &set, &oset);

	printf("Test sigprocmask oset...");
	//oset should be the previous mask
	for(sig = 0; sig < 32; sig++){
		if ( !((sig == SIGCHLD) || (sig == SIGKILL) || (sig == SIGSTOP)) ){
			if( sigismember(&oset, sig) != 1 ){
				printf("failed (%d is not set)\n", sig);
				return -1;
			}
		}
	}
	printf("passed\n");

	printf("Test sigprocmask oset SIGKILL...");
	if( sigismember(&oset, SIGKILL) == 1 ){
		printf("failed (SIGKILL is blocked)\n");
		return -1;
	}
	printf("passed\n");

	printf("Test sigprocmask oset SIGSTOP...");
	if( sigismember(&oset, SIGSTOP) == 1 ){
		printf("failed (SIGSTOP is blocked)\n");
		return -1;
	}
	printf("passed\n");


	printf("Test signal(200)...");
	errno = 0;
	if ( signal(200, signal_catching_function) != SIG_ERR ){
		printf("failed to detect invalid signal\n");
	}
	if ( errno != EINVAL ){
		fflush(stdout);
		perror("failed");
		return -1;
	}

	printf("detected failure, passed\n");

	for(sig = 0; sig < 32; sig++){
		printf("Test kill(%d)...", sig);
		if ( !((sig == SIGKILL) || (sig == SIGSTOP) || (sig == SIGCHLD)) ){
			sig_sent = -1;
			if ( kill(getpid(), sig) < 0 ){
				fflush(stdout);
				perror("failed\n");
			}

			if ( sig != 0 ){
				if ( sig_sent != sig ){
					printf("Failed to catch signal %d != %d\n", sig_sent, sig);
					return -1;
				} else {
					printf("passed\n");
				}
			} else {
				if ( sig_sent != -1 ){
					printf("Failed (signal 0 was caught)\n");
					return -1;
				} else {
					printf("no effect, passed\n");
				}
			}
		} else {
			printf("abort (SIGKILL, SIGCHLD, and SIGSTOP)\n");
		}
	}

	return 0;
}


void signal_catching_function(int sig){
	sig_sent = sig;
}

