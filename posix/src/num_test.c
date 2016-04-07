/*
 * num_test.c
 *
 *  Created on: May 25, 2013
 *      Author: tgil
 */


#include <stdio.h>
#include <math.h>


int float_test(void);
int int_test(void);

int num_test(void){

	int_test();
	float_test();

	return 0;
}

int int_test(void){
	volatile int x;
	int y;

	x = 2;

	printf("Start int tests\n");

	printf("test <...");
	if( x < 3 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	printf("test <=...");
	if( x <= 3 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	x = 4;
	printf("test >...");
	if( x > 3 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	printf("test >=...");
	if( x >= 3 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	printf("test ==...");
	if( x == 3 ){
		printf("failed\n");
		return -1;
	} else printf("passed\n");

	printf("test !=...");
	if( x != 3 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	y = x * 5;
	printf("test *...");
	if( y == 20 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	x = 100;
	y = x / 5;
	printf("test /...");
	if( y == 20 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	x = 4;
	y = x + 5;
	printf("test +...");
	if( y == 9 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	y = x - 5;
	printf("test -...");
	if( y == -1 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	return 0;
}

int float_test(void){
	volatile float x;
	float y;

	printf("Start float tests\n");
	x = 2.0;
	printf("done\n");

	printf("test <...");
	if( x < 3.0 ){
		printf("passed\n");
	} else {
		printf("failed\n");
		return -1;
	}

	printf("test <=...");
	if( x <= 3.0 ) printf("passed\n");
	else {
			printf("failed\n");
			return -1;
		}

	x = 4.0;
	printf("test >...");
	if( x > 3.0 ) printf("passed\n");
	else {
			printf("failed\n");
			return -1;
		}

	printf("test >=...");
	if( x >= 3.0 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	printf("test ==...");
	if( x == 3.0 ){
		printf("failed\n");
		return -1;
	} else printf("passed\n");

	printf("test !=...");
	if( x != 3.0 ) printf("passed\n");
	else printf("failed\n");

	y = x * 5.0;
	printf("test *...");
	if( y == 20.0 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	y = x / 5.0;
	printf("test /...");
	if( roundf(y*10.0) == (8.0) ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	y = x + 5.0;
	printf("test +...");
	if( y == 9.0 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	y = x - 5.0;
	printf("test -...");
	if( y == -1.0 ) printf("passed\n");
	else {
		printf("failed\n");
		return -1;
	}

	return 0;
}
