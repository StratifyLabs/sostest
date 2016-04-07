/*
 * set_get_test.c
 *
 *  Created on: Nov 11, 2011
 *      Author: tgil
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
