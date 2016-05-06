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

int aio_test(void);
int listio_test(void);
int dirent_test(void);
int mqueue_test(void);
int num_test(void);
int sched_test(void);
int sem_test(void);
int unistd_file_test(void);
int unistd_access_test(void);
int unistd_directory_test(void);
int unistd_sleep_test(void);

int stdio_test(void);

int signal_master_test(void);
int sigaction_test(void);
int signal_test(void);
int sigset_test(void);

int pthread_attr_test(void);
int pthread_condattr_test(void);
int pthread_mutexattr_test(void);
int pthread_rwlockattr_test(void);

int pthread_master_test(void); //general pthread test
int pthread_test(void); //general pthread test
int pthread_cond_test(void);
int pthread_mutex_test(void);
int pthread_rwlock_test(void);

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

int test_failed_perror(void);
void test_passed(void);
void test_intro(const char * func_name, const char * condition, const char * args);
int test_errno(int ret, const char * expected_errno, int exp_errno);
int test_success(int ret);

int launch_test(void);

extern char trace_buffer[32];
#define TRACE_MESSAGE(X) do { strncpy(trace_buffer, X, 32); posix_trace_event(POSIX_TRACE_MESSAGE, trace_buffer, strlen(trace_buffer)); } while(0)
#define TRACE_VALUE(X,Y) do { sprintf(trace_buffer, "%s-%d", X, Y); posix_trace_event(POSIX_TRACE_MESSAGE, trace_buffer, strlen(trace_buffer)); } while(0)


#endif /* TESTS_H_ */
