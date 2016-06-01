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



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "mcu/types.h"

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

static int float_test();
static int int_test();
static int mul_u64(operand_t a, operand_t b, operand_t result);

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

// u64
static int mul_u64(operand_t a, operand_t b, operand_t result);
static int div_u64(operand_t a, operand_t b, operand_t result);
static int cmp_u64(operand_t a, operand_t b, operand_t result);
static int sub_u64(operand_t a, operand_t b, operand_t result);
static int add_u64(operand_t a, operand_t b, operand_t result);
static int mod_u64(operand_t a, operand_t b, operand_t result);
static int gt_u64(operand_t a, operand_t b, operand_t result);
static int gte_u64(operand_t a, operand_t b, operand_t result);
static int lt_u64(operand_t a, operand_t b, operand_t result);
static int lte_u64(operand_t a, operand_t b, operand_t result);

//i64
static int mul_i64(operand_t a, operand_t b, operand_t result);
static int div_i64(operand_t a, operand_t b, operand_t result);
static int cmp_i64(operand_t a, operand_t b, operand_t result);
static int sub_i64(operand_t a, operand_t b, operand_t result);
static int add_i64(operand_t a, operand_t b, operand_t result);
static int mod_i64(operand_t a, operand_t b, operand_t result);
static int gt_i64(operand_t a, operand_t b, operand_t result);
static int gte_i64(operand_t a, operand_t b, operand_t result);
static int lt_i64(operand_t a, operand_t b, operand_t result);
static int lte_i64(operand_t a, operand_t b, operand_t result);


//float
static int multi_f(operand_t a, operand_t b, operand_t result);
static int divi_f(operand_t a, operand_t b, operand_t result);
static int cmpa_f(operand_t a, operand_t b, operand_t result);
static int sub_f(operand_t a, operand_t b, operand_t result);
static int add_f(operand_t a, operand_t b, operand_t result);
//static int mod_f(operand_t a, operand_t b, operand_t result);
static int gt_f(operand_t a, operand_t b, operand_t result);
static int gte_f(operand_t a, operand_t b, operand_t result);
static int lt_f(operand_t a, operand_t b, operand_t result);
static int lte_f(operand_t a, operand_t b, operand_t result);


//double
static int mul_d(operand_t a, operand_t b, operand_t result);
static int div_d(operand_t a, operand_t b, operand_t result);
static int cmp_d(operand_t a, operand_t b, operand_t result);
static int sub_d(operand_t a, operand_t b, operand_t result);
static int add_d(operand_t a, operand_t b, operand_t result);
//static int mod_d(operand_t a, operand_t b, operand_t result);
static int gt_d(operand_t a, operand_t b, operand_t result);
static int gte_d(operand_t a, operand_t b, operand_t result);
static int lt_d(operand_t a, operand_t b, operand_t result);
static int lte_d(operand_t a, operand_t b, operand_t result);

//float to int

static int f_to_int(operand_t a, operand_t b, operand_t result);
static int f_to_u64(operand_t a, operand_t b, operand_t result);
static int f_to_i64(operand_t a, operand_t b, operand_t result);
static int f_to_i32(operand_t a, operand_t b, operand_t result);
static int f_to_u32 (operand_t a, operand_t b, operand_t result);

static int int_to_f(operand_t a, operand_t b, operand_t result);
static int u64_to_f(operand_t a, operand_t b, operand_t result);
static int i64_to_f(operand_t a, operand_t b, operand_t result);
static int i32_to_f(operand_t a, operand_t b, operand_t result);
static int u32_to_f (operand_t a, operand_t b, operand_t result);




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

		{ .name = "cmp_u64 0 0", .operation = cmp_u64, .a.op_u64 = 0, .b.op_u64 = 0, .result.op_int = (0==0) },
		{ .name = "cmp_u64 0 100", .operation = cmp_u64, .a.op_u64 = 0, .b.op_u64 = 100, .result.op_int = (0==100) },
		{ .name = "cmp_u64 -100 100", .operation = cmp_u64, .a.op_u64 = -100, .b.op_u64 = 100, .result.op_int = (-100==100) },
		{ .name = "cmp_u64 100 -100", .operation = cmp_u64, .a.op_u64 = 100, .b.op_u64 = -100, .result.op_int = (100==-100) },
		{ .name = "cmp_u64 LLONG_MAX LLONG_MAX", .operation = cmp_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = LLONG_MAX, .result.op_int = (LLONG_MAX==LLONG_MAX) },
		{ .name = "cmp_u64 LLONG_MAX LLONG_MIN", .operation = cmp_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = LLONG_MIN, .result.op_int = (LLONG_MAX==LLONG_MIN) },

		{ .name = "gt_u64 0 0", .operation = gt_u64, .a.op_u64 = 0, .b.op_u64 = 0, .result.op_int = (0>0) },
		{ .name = "gt_u64 0 100", .operation = gt_u64, .a.op_u64 = 0, .b.op_u64 = 100, .result.op_int = (0>100) },
		{ .name = "gt_u64 INT_MAX INT_MAX", .operation = gt_u64, .a.op_u64 = INT_MAX, .b.op_u64 = INT_MAX, .result.op_int = (INT_MAX > INT_MAX) },
		{ .name = "gt_u64 INT_MAX LLONG_MAX", .operation = gt_u64, .a.op_u64 = INT_MAX, .b.op_u64 = LLONG_MAX, .result.op_int = (INT_MAX > LLONG_MAX) },
		{ .name = "gt_u64 LLONG_MAX LLONG_MAX", .operation = gt_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = LLONG_MAX, .result.op_int = (LLONG_MAX>LLONG_MAX) },
		{ .name = "gt_u64 LLONG_MAX INT_MAX", .operation = gt_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = INT_MAX, .result.op_int = (LLONG_MAX>INT_MAX) },

		{ .name = "gte_u64 0 0", .operation = gte_u64, .a.op_u64 = 0, .b.op_u64 = 0, .result.op_int = (0>=0) },
		{ .name = "gte_u64 0 100", .operation = gte_u64, .a.op_u64 = 0, .b.op_u64 = 100, .result.op_int = (0>=100) },
		{ .name = "gte_u64 INT_MAX INT_MAX", .operation = gte_u64, .a.op_u64 = INT_MAX, .b.op_u64 = INT_MAX, .result.op_int = (INT_MAX >= INT_MAX) },
		{ .name = "gte_u64 INT_MAX LLONG_MAX", .operation = gte_u64, .a.op_u64 = INT_MAX, .b.op_u64 = LLONG_MAX, .result.op_int = (INT_MAX >= LLONG_MAX) },
		{ .name = "gte_u64 LLONG_MAX LLONG_MAX", .operation = gte_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = LLONG_MAX, .result.op_int = (LLONG_MAX>=LLONG_MAX) },
		{ .name = "gte_u64 LLONG_MAX INT_MAX", .operation = gte_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = INT_MAX, .result.op_int = (LLONG_MAX>=INT_MAX) },

		{ .name = "mul_u64 100 100", .operation = mul_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100*100) },
		{ .name = "mul_u64 0 100", .operation = mul_u64, .a.op_u64 = 0, .b.op_u64 = 100, .result.op_u64 = (0*100) },
		{ .name = "mul_u64 100 0", .operation = mul_u64, .a.op_u64 = 100, .b.op_u64 = 0, .result.op_u64 = (0*100) },
		{ .name = "mul_u64 100 -100", .operation = mul_u64, .a.op_u64 = 100, .b.op_u64 = -100, .result.op_u64 = ((u64)100*-100) },
		{ .name = "mul_u64 LLONG_MAX 0", .operation = mul_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = 0, .result.op_u64 = ((u64)LLONG_MAX*0) },
		{ .name = "mul_u64 INT_MAX INT_MAX", .operation = mul_u64, .a.op_u64 = INT_MAX, .b.op_u64 = INT_MAX, .result.op_u64 = ((u64)(INT_MAX)*(INT_MAX)) },
		{ .name = "div_u64 LLONG_MAX 100", .operation = div_u64, .a.op_u64 = LLONG_MAX, .b.op_u64 = 100, .result.op_u64 = ((u64)LLONG_MAX/100) },
		{ .name = "div_u64 100 100", .operation = div_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100/100) },
		{ .name = "sub_u64 100 100", .operation = sub_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100-100) },
		{ .name = "add_u64 100 100", .operation = add_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100+100) },
		{ .name = "mod_u64 100 100", .operation = mod_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100%100) },
		{ .name = "lt_u64 100 100", .operation = lt_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100<100) },
		{ .name = "lte_u64 100 100", .operation = lte_u64, .a.op_u64 = 100, .b.op_u64 = 100, .result.op_u64 = (100<=100) },



		{ .name = "mul_i64 100 100", .operation = mul_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100*100) },
		{ .name = "mul_i64 -100 100", .operation = mul_i64, .a.op_i64 = -100, .b.op_i64 = 100, .result.op_i64 = (-100*100) },
		{ .name = "div_i64 100 100", .operation = div_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100/100) },
		{ .name = "cmp_i64 100 100", .operation = cmp_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100==100) },
		{ .name = "sub_i64 100 100", .operation = sub_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100-100) },
		{ .name = "sub_i64 LLONG_MAX 1024", .operation = sub_i64, .a.op_i64 = LLONG_MAX, .b.op_i64 = 1024, .result.op_i64 = ((i64)LLONG_MAX-1024) },
		{ .name = "sub_i64 LLONG_MAX LLONG_MAX", .operation = sub_i64, .a.op_i64 = LLONG_MAX, .b.op_i64 = LLONG_MAX, .result.op_i64 = ((i64)LLONG_MAX-LLONG_MAX) },
		{ .name = "sub_i64 0 LLONG_MAX", .operation = sub_i64, .a.op_i64 = 0, .b.op_i64 = LLONG_MAX, .result.op_i64 = ((i64)0-LLONG_MAX) },
		{ .name = "sub_i64 INT_MIN INT_MAX", .operation = sub_i64, .a.op_i64 = INT_MIN, .b.op_i64 = INT_MAX, .result.op_i64 = ((i64)INT_MIN-INT_MAX) },
		{ .name = "add_i64 INT_MIN INT_MAX", .operation = add_i64, .a.op_i64 = INT_MIN, .b.op_i64 = INT_MAX, .result.op_i64 = ((i64)INT_MIN+INT_MAX) },
		{ .name = "add_i64 100 INT_MAX", .operation = add_i64, .a.op_i64 = 100, .b.op_i64 = INT_MAX, .result.op_i64 = ((i64)100+INT_MAX) },
		{ .name = "add_i64 INT_MAX -1", .operation = add_i64, .a.op_i64 = INT_MAX, .b.op_i64 = -1, .result.op_i64 = ((i64)INT_MAX+-1) },
		{ .name = "add_i64 INT_MAX INT_MAX/2", .operation = add_i64, .a.op_i64 = INT_MAX, .b.op_i64 = INT_MAX/2, .result.op_i64 = ((i64)INT_MAX+INT_MAX/2) },
		{ .name = "add_i64 INT_MAX INT_MAX", .operation = add_i64, .a.op_i64 = INT_MAX, .b.op_i64 = INT_MAX, .result.op_i64 = ((i64)INT_MAX+INT_MAX) },
		{ .name = "add_i64 LLONG_MAX -100", .operation = add_i64, .a.op_i64 = LLONG_MAX, .b.op_i64 = -100, .result.op_i64 = ((i64)LLONG_MAX+-100) },
		{ .name = "add_i64 LLONG_MAX LLONG_MIN", .operation = add_i64, .a.op_i64 = LLONG_MAX, .b.op_i64 = LLONG_MIN, .result.op_i64 = ((i64)LLONG_MAX+LLONG_MIN) },

		{ .name = "mod_i64 100 100", .operation = mod_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100%100) },
		{ .name = "gt_i64 100 100", .operation = gt_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_int = (100>100) },
		{ .name = "gt_i64 -100 100", .operation = gt_i64, .a.op_i64 = -100, .b.op_i64 = 100, .result.op_int = (-100>100) },
		{ .name = "gt_i64 100 -100", .operation = gt_i64, .a.op_i64 = 100, .b.op_i64 = -100, .result.op_int = (100>-100) },
		{ .name = "gt_i64 0 LLONG_MAX", .operation = gt_i64, .a.op_i64 = 0, .b.op_i64 = LLONG_MAX, .result.op_int = (0>LLONG_MAX) },
		{ .name = "gt_i64 LLONG_MAX -1", .operation = gt_i64, .a.op_i64 = LLONG_MAX, .b.op_i64 = -1, .result.op_int = (LLONG_MAX>-1) },
		{ .name = "gt_i64 LLONG_MAX LLONG_MIN", .operation = gt_i64, .a.op_i64 = LLONG_MAX, .b.op_i64 = LLONG_MIN, .result.op_int = (LLONG_MAX>LLONG_MIN) },
		{ .name = "gt_i64 LLONG_MIN LLONG_MAX", .operation = gt_i64, .a.op_i64 = LLONG_MIN, .b.op_i64 = LLONG_MAX, .result.op_int = (LLONG_MIN>LLONG_MAX) },
		{ .name = "gte_i64 100 100", .operation = gte_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100>=100) },
		{ .name = "lt_i64 100 100", .operation = lt_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100<100) },
		{ .name = "lte_i64 100 100", .operation = lte_i64, .a.op_i64 = 100, .b.op_i64 = 100, .result.op_i64 = (100<=100) },


		{ .name = "multi_f 100.0 100.0", .operation = multi_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_f = (100.0*100.0) },
		{ .name = "divi_f 100.0 100.0", .operation = divi_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_f = (100.0/100.0) },
		{ .name = "cmpa_f 100.0 100.0", .operation = cmpa_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_int = (100.0==100.0) },
		{ .name = "sub_f 100.0 100.0", .operation = sub_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_f = (100.0-100.0) },
		{ .name = "add_f 100.0 100.0", .operation = add_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_f = (100.0+100.0) },
		{ .name = "gt_f 100.0 100.0", .operation = gt_f, .a.op_f = 100, .b.op_f = 100.0, .result.op_f = (100.0>100.0) },
		{ .name = "gte_f 100.0 100.0", .operation = gte_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_int = (100.0>=100.0) },
		{ .name = "lt_f 100.0 100.0", .operation = lt_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_f = (100.0<100.0) },
		{ .name = "lte_f 100.0 100.0", .operation = lte_f, .a.op_f = 100.0, .b.op_f = 100.0, .result.op_int = (100.0<=100.0) },


		{ .name = "mul_d 100.0 100.0", .operation = mul_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0*100.0) },
		{ .name = "div_d 100.0 100.0", .operation = div_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0/100.0) },
		{ .name = "cmp_d 100.0 100.0", .operation = cmp_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_int = (100.0==100.0) },
		{ .name = "sub_d 100.0 100.0", .operation = sub_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0-100.0) },
		{ .name = "add_d 100.0 100.0", .operation = add_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0+100.0) },
		{ .name = "gt_d 100.0 100.0", .operation = gt_d, .a.op_d = 100, .b.op_d = 100.0, .result.op_d = (100.0>100.0) },
		{ .name = "gte_d 100.0 100.0", .operation = gte_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_int = (100.0>=100.0) },
		{ .name = "lt_d 100.0 100.0", .operation = lt_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0<100.0) },
		{ .name = "lte_d 100.0 100.0", .operation = lte_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_int = (100.0<=100.0) },

		{ .name = "f_to_int 0.0", .operation = f_to_int, .a.op_f = 0.0, .b.op_f = 0.0 , .result.op_int = 0 },
		{ .name = "f_to_int 0.2", .operation = f_to_int, .a.op_f = 0.2, .b.op_f = 0.0 , .result.op_int = 0 },
		{ .name = "f_to_int 0.6", .operation = f_to_int, .a.op_f = 0.6, .b.op_f = 0.0 , .result.op_int = 0 },
		{ .name = "f_to_int 0.8", .operation = f_to_int, .a.op_f = 0.8, .b.op_f = 0.0 , .result.op_int = 0 },
		{ .name = "f_to_int 1.9", .operation = f_to_int, .a.op_f = 1.9, .b.op_f = 0.0 , .result.op_int = 1 },
		{ .name = "f_to_int -2.987", .operation = f_to_int, .a.op_f = -2.987, .b.op_f = 0.0 , .result.op_int = -2 },
		{ .name = "f_to_i64 -27.2", .operation = f_to_i64, .a.op_f = -27.2, .b.op_f = 0.0 , .result.op_i64 = -27 },
		{ .name = "f_to_u64 42.907", .operation = f_to_u64, .a.op_f = 42.907, .b.op_f = 0.0 , .result.op_u64 = 42 },
		{ .name = "f_to_i32 HUGE_VAL", .operation = f_to_i32, .a.op_f = HUGE_VAL, .b.op_f = 0.0 , .result.op_i32 = INT_MAX },
		{ .name = "f_to_i32 -HUGE_VAL", .operation = f_to_i32, .a.op_f = -HUGE_VAL, .b.op_f = 0.0 , .result.op_i32 = INT_MIN },
		{ .name = "f_to_i32 7.2", .operation = f_to_i32, .a.op_f = 7.2, .b.op_f = 0.0 , .result.op_i32 = 7 },
		{ .name = "f_to_u32 0.6", .operation = f_to_u32, .a.op_f = 0.6, .b.op_f = 0.0 , .result.op_u32 = 0 },


		{ .name = "u32_to_f 15", .operation = u32_to_f, .a.op_u32 = 15, .b.op_int = 0 , .result.op_f = 15.0 },
		{ .name = "i32_to_f -15", .operation = i32_to_f, .a.op_i32 = -15, .b.op_int = 0 , .result.op_f = -15.0 },
		{ .name = "u64_to_f 15", .operation = u64_to_f, .a.op_u64 = 15, .b.op_int = 0 , .result.op_f = 15.0 },
		{ .name = "i64_to_f -15", .operation = i64_to_f, .a.op_i64 = -15, .b.op_int = 0 , .result.op_f = -15.0 },
		{ .name = "int_to_f 15", .operation = int_to_f, .a.op_int = 15, .b.op_int = 0 , .result.op_f = 15.0 },

		{ .name = "mul_d -0.125 HUGE_VAL", .operation = mul_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
		{ .name = "div_d  HUGE_VAL -0.125", .operation = div_d, .a.op_d = HUGE_VAL, .b.op_d = -0.125, .result.op_d = -INFINITY },
		{ .name = "cmp_d -0.125 HUGE_VAL", .operation = cmp_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
		{ .name = "sub_d -0.125 HUGE_VAL", .operation = sub_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
		{ .name = "add_d -0.125 HUGE_VAL", .operation = add_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = INFINITY },
		{ .name = "gt_d -0.125 HUGE_VAL", .operation = gt_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
		{ .name = "gte_d -0.125 HUGE_VAL", .operation = gte_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
		{ .name = "lt_d HUGE_VAL -0.125", .operation = lt_d, .a.op_d = HUGE_VAL, .b.op_d = -0.125, .result.op_d = -INFINITY },
		{ .name = "lte_d  HUGE_VAL -0.125", .operation = lte_d, .a.op_d = HUGE_VAL, .b.op_d = -0.125, .result.op_d = -INFINITY },


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

int num_test(){
	int_test();
	float_test();

	int i;
	int total = sizeof(tests) / sizeof(test_t);
	for(i=0; i < total; i++){
		exec_test(i);
	}



	return 0;
}



int int_test(){
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

int float_test(){
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
	//	if( roundf(y*10.0) == (8.0) )
	//		printf("passed\n");
	//	else {
	//		printf("failed\n");
	//		return -1;
	//	}

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




/*
 * Float Multiplication
 */
/*	int mul_f(operand_t a, operand_t b, operand_t result)
	{
		operand_t t;

		t.op_u64 = a.op_u64 * b.op_u64;

		if( t.op_u64 == result.op_u64 )
		{
		return 1;
		}

		printf("%f != %f...", t.op_u64, result.op_u64);
		return 0;
		}

 */

////u64

/*
 *  Integer Multiplication
 */
int mul_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_u64 = a.op_u64 * b.op_u64;

	if( t.op_u64 == result.op_u64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}

/*
 *  Integer Substraction
 */
int sub_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_u64 = a.op_u64 - b.op_u64;

	if( t.op_u64 == result.op_u64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}




/*
 *  Integer Addition
 */
int add_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_u64 = a.op_u64 + b.op_u64;

	if( t.op_u64 == result.op_u64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}



/*
 *  Integer Division
 */
int div_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	lldiv_t o;
	//t.op_u64 = (u64)a.op_u64 / (u64)b.op_u64;

	o = lldiv(a.op_u64, b.op_u64);
	t.op_u64 = o.quot;

	if( t.op_u64 == result.op_u64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}



/*
 *  Integer Comparision
 */
int cmp_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if( a.op_u64 == b.op_u64 )
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}


/*
 *  Integer modulus
 */
int mod_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	lldiv_t o;
	o = lldiv(a.op_u64, b.op_u64);
	t.op_u64 = o.rem;


	if( t.op_u64 == result.op_u64 ){
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;

}


/*
 *  Integer Greaterthan
 */
int gt_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if(a.op_u64 > b.op_u64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int ){
		return 1;
	}

	printf("%lld", a.op_u64);  fflush(stdout);
	printf(" != %lld", b.op_u64); fflush(stdout);
	printf("...%d != %d...", t.op_int, result.op_int); fflush(stdout);

	return 0;
}


/*
 *  Integer Greater than or equal
 */
int gte_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_u64 >= b.op_u64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}


/*
 *  Integer Less than
 */
int lt_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_u64 < b.op_u64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}


	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}


/*
 *  Integer Less than or equal
 */
int lte_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_u64 <= b.op_u64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}


///i64


/*
 *  Integer Multiplication
 */
int mul_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_i64 = a.op_i64 * b.op_i64;

	if( t.op_i64 == result.op_i64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}

/*
 *  Integer Substraction
 */
int sub_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_i64 = a.op_i64 - b.op_i64;

	if( t.op_i64 == result.op_i64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}


/*
 *  Integer Addition
 */
int add_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_i64 = a.op_i64 + b.op_i64;

	if( t.op_i64 == result.op_i64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}



/*
 *  Integer Division
 */
int div_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	lldiv_t o;
	o = lldiv(a.op_i64, b.op_i64);
	t.op_u64 = o.quot;

	//t.op_i64 = a.op_i64 / b.op_i64;

	if( t.op_i64 == result.op_i64 )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_u64, result.op_u64);
	return 0;
}



/*
 *  Integer Comparision
 */
int cmp_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if( a.op_i64 == b.op_i64 )
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}


/*
 *  Integer modulus
 */
int mod_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	//t.op_i64 = a.op_i64 % b.op_i64;

	lldiv_t o;
	o = lldiv(a.op_i64, b.op_i64);
	t.op_i64 = o.rem;

	if( t.op_i64 == result.op_i64 ){
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}


/*
 *  Integer Greaterthan
 */
int gt_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if(a.op_i64 > b.op_i64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}


/*
 *  Integer Greater than or equal
 */
int gte_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_i64 >= b.op_i64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}


/*
 *  Integer Lessthan
 */
int lt_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_i64 < b.op_i64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}


	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}


/*
 *  Integer Less than or equal
 */
int lte_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_i64 <= b.op_i64)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lld != %lld...", t.op_i64, result.op_i64);
	return 0;
}

//float

/*
 *  Float Multiplication
 */
int multi_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_f = a.op_f * b.op_f;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}

/*
 *  Float Substraction
 */
int sub_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_f = a.op_f - b.op_f;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}




/*
 *  Float Addition
 */
int add_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_f = a.op_f + b.op_f;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}



/*
 *  Float Division
 */
int divi_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_f = a.op_f / b.op_f;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}



/*
 *  Float Comparision
 */
int cmpa_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if( a.op_f == b.op_f )
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}



/*
 *  Float Greater than
 */
int gt_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if(a.op_f > b.op_f)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}


/*
 *  Float Greater than or equal
 */
int gte_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_f >= b.op_f)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}


/*
 *  Float Less than
 */
int lt_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_f < b.op_f)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}


	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}


/*
 *  Float Less than or equal
 */
int lte_f(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_f <= b.op_f)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%f != %f...", t.op_f, result.op_f);
	return 0;
}

//Double

/*
 *  Double Multiplication
 */
int mul_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_d = a.op_d * b.op_d;

	if( t.op_d == result.op_d )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}

/*
 *  Double Substraction
 */
int sub_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_d = a.op_d - b.op_d;

	if( t.op_d == result.op_d )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}




/*
 *  Double Addition
 */
int add_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_d = a.op_d + b.op_d;

	if( t.op_d == result.op_d )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}



/*
 *  Double Division
 */
int div_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_d = a.op_d / b.op_d;

	if( t.op_d == result.op_d )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}



/*
 *  Double Comparision
 */
int cmp_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if( a.op_d == b.op_d )
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}


/*
 *  Double Greater than
 */
int gt_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;
	if(a.op_d > b.op_d)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}


/*
 *  Double Greater than or equal
 */
int gte_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_d >= b.op_d)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}


/*
 *  Double Less than
 */
int lt_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_d < b.op_d)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}


	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}


/*
 *  Double Less than or equal
 */
int lte_d(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	if(a.op_d <= b.op_d)
	{
		t.op_int = 1;
	}
	else
	{
		t.op_int = 0;
	}

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%lf != %lf...", t.op_d, result.op_d);
	return 0;
}


//float to int

int f_to_int(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_int = (int)a.op_f;

	if( t.op_int == result.op_int )
	{
		return 1;
	}

	printf("%d != %f...", t.op_int, a.op_f);
	return 0;
}

//		float to u64
int f_to_u64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_u64 = (u64)a.op_f;

	if( t.op_u64 == result.op_u64 )
	{
		return 1;
	}

	printf("%lld != %f...", t.op_u64, a.op_f);
	return 0;
}


//		float to i64

int f_to_i64(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_i64 = (i64)a.op_f;

	if( t.op_i64 == result.op_i64 )
	{
		return 1;
	}

	printf("%lld != %f...", t.op_i64, a.op_f);
	return 0;
}


//		float to i32

int f_to_i32(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_i32 = (i32)a.op_f;

	if( t.op_i32 == result.op_i32 )
	{
		return 1;
	}

	printf("%ld != %f...", t.op_i32, a.op_f);
	return 0;
}


//		float to u32
int f_to_u32(operand_t a, operand_t b, operand_t result)
{
	operand_t t;

	t.op_u32 = (u32)a.op_f;

	if( t.op_u32 == result.op_u32 )
	{
		return 1;
	}

	printf("%ld != %f...", t.op_u32, a.op_f);
	return 0;
}

int int_to_f(operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = (float)a.op_int;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %d...", t.op_f, a.op_int);
	return 0;
}

int u64_to_f(operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = (float)a.op_u64;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %lld...", t.op_f, a.op_u64);
	return 0;
}

int i64_to_f(operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = (float)a.op_i64;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %lld...", t.op_f, a.op_i64);
	return 0;
}

int i32_to_f(operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = (float)a.op_i32;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %ld...", t.op_f, a.op_i32);
	return 0;
}

int u32_to_f (operand_t a, operand_t b, operand_t result){
	operand_t t;

	t.op_f = (float)a.op_u32;

	if( t.op_f == result.op_f )
	{
		return 1;
	}

	printf("%f != %ld...", t.op_f, a.op_u32);
	return 0;
}

