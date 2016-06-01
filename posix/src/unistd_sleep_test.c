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

#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include "tests.h"

#define SLEEP_TEST 1
#define USLEEP_TEST 1

static int calc_useconds(struct timespec * start, struct timespec * stop);
static int usleep_test_case(int useconds, int ref);
static int sleep_test_case(int seconds);
static int usleep_test();

int unistd_sleep_test(){
	int i;
	int tmp;
	bool failed;

	if ( SLEEP_TEST ){
		failed = false;
		for(i=1; i < 4; i+= 1){
			printf("Test sleep(%d)...", i);
			fflush(stdout);
			tmp = sleep_test_case(i);
			if ( tmp < 0 ){
				printf("failed underslept by %d\n", 0 - tmp);
				failed = true;
			} else {
				printf("passed\n");
			}
		}
	}

	if ( failed == true ){
		return -1;
	}

	if ( USLEEP_TEST ){
		if ( usleep_test() < 0 ){
			return -1;
		}
	}

	return 0;
}

int usleep_test(){
	int i;
	int ref;
	int actual;
	struct timespec start;
	struct timespec stop;
	bool failed;

	clock_gettime(CLOCK_REALTIME, &start);
	clock_gettime(CLOCK_REALTIME, &stop);

	ref = calc_useconds(&start, &stop);
	ref = ref / 1000; //take the value in microseconds

	failed = false;
	//printf("Start: %d %d Stop: %d %d Ref %d\n", start.tv_sec, start.tv_nsec, stop.tv_sec, stop.tv_nsec, ref);
	for(i=1; i < 1000; i++){
		printf("Test usleep(%d)...", i);
		fflush(stdout);

		actual = usleep_test_case(i, ref);
		if ( actual < i ){
			printf("failed underslept by %d\n", i - actual);
			failed = true;
		} else {
			printf("overslept by %d...passed\n", actual - i);
		}
	}
	if ( failed == true ){
		return -1;
	}
	return 0;
}


static int usleep_test_case(int useconds, int ref){
	struct timespec start;
	struct timespec stop;
	int utmp;
	int tmp;

	clock_gettime(CLOCK_REALTIME, &start);
	usleep(useconds);
	clock_gettime(CLOCK_REALTIME, &stop);

	tmp = calc_useconds(&start, &stop);
	utmp = tmp - ref;
	return utmp;
}

static int sleep_test_case(int seconds){
	struct timespec start;
	struct timespec stop;
	int tmp;

	clock_gettime(CLOCK_REALTIME, &start);
	sleep(seconds);
	clock_gettime(CLOCK_REALTIME, &stop);


	tmp = calc_useconds(&start, &stop);
	tmp = (tmp + 500000) / 1000000;
	return tmp - seconds;
}

int calc_useconds(struct timespec * start, struct timespec * stop){
	int ret;
	if ( start->tv_sec == stop->tv_sec ){
		ret = (stop->tv_nsec + 500)/1000 - (start->tv_nsec + 500)/1000;
	} else {
		ret = (stop->tv_nsec + 500)/1000 - (start->tv_nsec+500)/1000 + 1000000;
		ret += ((stop->tv_sec - start->tv_sec)*1000000);
	}
	return ret;
}
