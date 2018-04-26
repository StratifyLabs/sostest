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

#ifndef TESTS_H_
#define TESTS_H_

#include <trace.h>

#if defined __cplusplus
extern "C" {
#endif


int aio_test();
int listio_test();
int dirent_test();
int mqueue_test();
int num_test();
int sched_test();
int sem_test();
int unistd_file_test();
int unistd_access_test();
int unistd_directory_test();
int unistd_sleep_test();

int stdio_test();

int signal_master_test();
int sigaction_test();
int signal_test();
int sigset_test();

int pthread_attr_test();
int pthread_condattr_test();
int pthread_mutexattr_test();
int pthread_rwlockattr_test();

int pthread_master_test(); //general pthread test
int pthread_test(); //general pthread test
int pthread_cond_test();
int pthread_mutex_test();
int pthread_rwlock_test();

int create_thread(void * (*func)(void*), int stacksize, int detachstate);

typedef struct {
	int case_number;
	int err_number;
} test_case_t;

int set_get_test(const char * name,
		int (*set_function)(void *, int),
		int (*get_function)(void*, int*),
		void * attr,
		test_case_t cases[],
		int total,
		int verbose
		);

int create_thread(void * (*func)(void*), int stacksize, int detachstate);

int test_failed_perror();
void test_passed();
void test_intro(const char * func_name, const char * condition, const char * args);
int test_errno(int ret, const char * expected_errno, int exp_errno);
int test_success(int ret);

int launch_test();

extern char trace_buffer[32];
#define TRACE_MESSAGE(X) do { strncpy(trace_buffer, X, 32); posix_trace_event(POSIX_TRACE_MESSAGE, trace_buffer, strlen(trace_buffer)); } while(0)
#define TRACE_VALUE(X,Y) do { sprintf(trace_buffer, "%s-%d", X, Y); posix_trace_event(POSIX_TRACE_MESSAGE, trace_buffer, strlen(trace_buffer)); } while(0)

#if defined __cplusplus
}
#endif

#endif /* TESTS_H_ */
