/*
 * num_test.c
 *
 *  Created on: May 25, 2013
 *      Author: tgil
 */


#include <mcu/types.h>
#include <stdio.h>
#include <math.h>


static int float_test(void);
static int int_test(void);

typedef union {
	float op_f;
	double op_d;
	int op_int;
	u8 op_u8;
	u16 op_u16;
	u32 op_u32;
	u64 op_u64;
	i8 op_i8;
	i16 op_i16;
	i32 op_i32;
	i64 op_i64;
} operand_t;

typedef int (*op)(operand_t, operand_t, operand_t);

typedef struct {
	const char * name;
	op operation;
	operand_t a;
	operand_t b;
	operand_t result;
} test_t;


static int mul_f(operand_t a, operand_t b, operand_t result);
static int div_f(operand_t a, operand_t b, operand_t result);
static int cmp_f(operand_t a, operand_t b, operand_t result);

const test_t tests[] = {
		{ .name = "cmp_f 1.0==1.0", .operation = cmp_f, .a.op_f = 1.0, .b.op_f = 1.0, .result.op_int = 1 },
		{ .name = "cmp_f 1234.567==765.4321", .operation = cmp_f, .a.op_f = 1234.567, .b.op_f = 765.4321, .result.op_int = 0 },
		{ .name = "cmp_f -123.0==-768.0", .operation = cmp_f, .a.op_f = -123.0, .b.op_f = -768.0, .result.op_int = 0 },
		{ .name = "cmp_f eq 0.0 -0.0", .operation = cmp_f, .a.op_f = 0.0, .b.op_f = -0.0, .result.op_int = 1 },
		{ .name = "cmp_f ne 1.0 nan", .operation = cmp_f, .a.op_f = 1.0, .b.op_f = NAN, .result.op_int = 0 },
		{ .name = "cmp_f ne nan 1.0", .operation = cmp_f, .a.op_f = NAN, .b.op_f = 1.0, .result.op_int = 0 },
		{ .name = "cmp_f ne nan nan", .operation = cmp_f, .a.op_f = NAN, .b.op_f = NAN, .result.op_int = 0 },
		{ .name = "cmp_f ne inf 1.0", .operation = cmp_f, .a.op_f = INFINITY, .b.op_f = 1.0, .result.op_int = 0 },
		{ .name = "cmp_f ne 0.0 inf", .operation = cmp_f, .a.op_f = 0.0, .b.op_f = INFINITY, .result.op_int = 0 },
		{ .name = "cmp_f ne -inf 0.0", .operation = cmp_f, .a.op_f = -INFINITY, .b.op_f = 0.0, .result.op_int = 0 },
		{ .name = "cmp_f ne 0.0 -inf", .operation = cmp_f, .a.op_f = 0.0, .b.op_f = -INFINITY, .result.op_int = 0 },
		{ .name = "cmp_f eq inf inf", .operation = cmp_f, .a.op_f = INFINITY, .b.op_f = INFINITY, .result.op_int = 1 },
		{ .name = "cmp_f eq -inf -inf", .operation = cmp_f, .a.op_f = -INFINITY, .b.op_f = -INFINITY, .result.op_int = 1 },
		{ .name = "cmp_f ne inf -inf", .operation = cmp_f, .a.op_f = INFINITY, .b.op_f = -INFINITY, .result.op_int = 0 },
		{ .name = "cmp_f ne -inf inf", .operation = cmp_f, .a.op_f = -INFINITY, .b.op_f = INFINITY, .result.op_int = 0 },
		{ .name = "cmp_f ne -inf inf", .operation = cmp_f, .a.op_f = -INFINITY, .b.op_f = INFINITY, .result.op_int = 0 },
		{ .name = "mul_f 0.5 10.0", .operation = mul_f, .a.op_f = 0.5, .b.op_f = 10.0, .result.op_f = 5.0 },
		{ .name = "mul_f -0.5 -2.0", .operation = mul_f, .a.op_f = -0.5, .b.op_f = -2.0, .result.op_f = 1.0 },
		{ .name = "mul_f HUGE_VALF 0.25", .operation = mul_f, .a.op_f = HUGE_VALF, .b.op_f = 0.25, .result.op_f = INFINITY },
		{ .name = "mul_f -0.125 HUGE_VALF", .operation = mul_f, .a.op_f = -0.125, .b.op_f = HUGE_VALF, .result.op_f = -INFINITY },
		{ .name = "mul_f 0.0 -0.0", .operation = mul_f, .a.op_f = 0.0, .b.op_f = -0.0, .result.op_f = 0.0 },
		{ .name = "div_f 1.0 1.0", .operation = div_f, .a.op_f = 1.0, .b.op_f = 1.0, .result.op_f = 1.0 },
};



static int exec_test(int i){
	printf("test %s...", tests[i].name); fflush(stdout);
	if( tests[i].operation(tests[i].a, tests[i].b, tests[i].result) == 0 ){
		printf("failed\n");
		return -1;
	} else {
		printf("passed\n");
	}
	return 0;
}

int num_test(void){
	int_test();
	float_test();

	int i;
	int total = sizeof(tests) / sizeof(test_t);
	for(i=0; i < total; i++){
		exec_test(i);
	}



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

int div_f(operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = a.op_f / b.op_f;

	if( t.op_f == result.op_f ){
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}

int mul_f(operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = a.op_f * b.op_f;

	if( t.op_f == result.op_f ){
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}

int cmp_f(operand_t a, operand_t b, operand_t result){
	operand_t t;
	if( a.op_f == b.op_f ){
		t.op_int = 1;
	} else {
		t.op_int = 0;
	}

	if( t.op_int == result.op_int ){
		return 1;
	}

	return 0;
}
