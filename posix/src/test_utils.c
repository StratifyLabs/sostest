/*
 * test_utils.c
 *
 *  Created on: Nov 30, 2013
 *      Author: tgil
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "tests.h"



int test_failed_perror(void){
	fflush(stdout);
	perror("failed");
	return -1;
}

void test_passed(void){
	printf("passed\n");
}

void test_intro(const char * func_name, const char * condition, const char * args){
	printf("Test %s", func_name);

	if( args != 0 ){
		printf("(%s)", args);
	} else {
		printf("()");
	}

	if( condition != 0 ){
		printf(" %s", condition);
	}
	printf("...");
}


int test_errno(int ret, const char * expected_errno, int exp_errno){
	if( ret < 0 ){
		if( exp_errno == errno ){
			test_passed();
			return 0;
		} else {
			printf("Failed (errno should have been %s not %d)\n", expected_errno, errno);
			return -1;
		}
	}

	printf("Failed -- should have failed with %s\n", expected_errno);
	return -1;
}

int test_success(int ret){
	if( ret < 0 ){
		test_failed_perror();
		return -1;
	}
	test_passed();
	return 0;
}
