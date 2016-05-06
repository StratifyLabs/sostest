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


int set_get_test(const char * name,
		int (*set_function)(void *, int),
		int (*get_function)(void*, int*),
		void * attr,
		test_case_t cases[],
		int total,
		int verbose
		){

	int tmp;
	int i;
	printf("Test %s...", name);
	for(i = 0; i < total; i++){
		if ( verbose ){
			fflush(stdout);
			printf("Case %d...", cases[i].case_number);
		}
		tmp = cases[i].case_number;
		errno = 0;
		set_function(attr, tmp);
		if ( cases[i].err_number != errno ){
			if ( errno == 0 ){
				printf("set should have failed with %d\n", cases[i].err_number);
			} else {
				fflush(stdout);
				perror("failed");
			}
			return -1;
		}

		errno = 0;
		if( get_function(attr, &tmp) ){
			fflush(stdout);
			perror("failed");
			return -1;
		}

		if ( cases[i].err_number == 0 ){
			if ( tmp != cases[i].case_number ){
				printf("failed set/get (%d != %d)\n", tmp, cases[i].case_number);
				return -1;
			}
		}
	}
	printf("passed\n");
	return 0;
}
