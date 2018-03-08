


#include <stdio.h>
#include <math.h>

#include <mcu/types.h>
#include "num_test.h"

typedef union {
    float op_f;
    double op_d;
    int op_int;
    u8 op_u8;
    u16 op_u16;
    u32 op_u32;
    u64 op_u64;
    i8 op_i8;
    s16 op_i16;
    s32 op_s32;
    s64 op_s64;
} operand_t;

static int float_test(void);
static int int_test(void);
static int mul_u64(operand_t a, operand_t b, operand_t result);

typedef int (*op)(operand_t, operand_t, operand_t);

typedef struct {
    const char * name;
    const char * a_str;
    const char * b_str;
    const char * op_str;
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
static int modulus_u64(operand_t a, operand_t b, operand_t result);
static int greaterthan_u64(operand_t a, operand_t b, operand_t result);
static int greaterthanorequal_u64(operand_t a, operand_t b, operand_t result);
static int lessthan_u64(operand_t a, operand_t b, operand_t result);
static int lessthanorequal_u64(operand_t a, operand_t b, operand_t result);

//s64
static int mul_s64(operand_t a, operand_t b, operand_t result);
static int div_s64(operand_t a, operand_t b, operand_t result);
static int cmp_s64(operand_t a, operand_t b, operand_t result);
static int sub_s64(operand_t a, operand_t b, operand_t result);
static int add_s64(operand_t a, operand_t b, operand_t result);
static int modulus_s64(operand_t a, operand_t b, operand_t result);
static int greaterthan_s64(operand_t a, operand_t b, operand_t result);
static int greaterthanorequal_s64(operand_t a, operand_t b, operand_t result);
static int lessthan_s64(operand_t a, operand_t b, operand_t result);
static int lessthanorequal_s64(operand_t a, operand_t b, operand_t result);

//float
static int multi_f(operand_t a, operand_t b, operand_t result);
static int divi_f(operand_t a, operand_t b, operand_t result);
static int cmpa_f(operand_t a, operand_t b, operand_t result);
static int sub_f(operand_t a, operand_t b, operand_t result);
static int add_f(operand_t a, operand_t b, operand_t result);
static int greaterthan_f(operand_t a, operand_t b, operand_t result);
static int greaterthanorequal_f(operand_t a, operand_t b, operand_t result);
static int lessthan_f(operand_t a, operand_t b, operand_t result);
static int lessthanorequal_f(operand_t a, operand_t b, operand_t result);

//double
static int mul_d(operand_t a, operand_t b, operand_t result);
static int div_d(operand_t a, operand_t b, operand_t result);
static int cmp_d(operand_t a, operand_t b, operand_t result);
static int sub_d(operand_t a, operand_t b, operand_t result);
static int add_d(operand_t a, operand_t b, operand_t result);
static int greaterthan_d(operand_t a, operand_t b, operand_t result);
static int greaterthanorequal_d(operand_t a, operand_t b, operand_t result);
static int lessthan_d(operand_t a, operand_t b, operand_t result);
static int lessthanorequal_d(operand_t a, operand_t b, operand_t result);

//float to int
static int f_to_int(operand_t a, operand_t b, operand_t result);
static int f_to_u64(operand_t a, operand_t b, operand_t result);
static int f_to_s64(operand_t a, operand_t b, operand_t result);
static int f_to_s32(operand_t a, operand_t b, operand_t result);
static int f_to_u32 (operand_t a, operand_t b, operand_t result);

//int to float
//double to int
//int to double

#define TEST_CASE(function_name, op_type, result_type, a_value, b_value, operation_value) \
{ .name = #function_name, .a_str = #a_value, .b_str = #b_value, .op_str = #operation_value, .operation = function_name, .a.op_type = a_value, .b.op_type = b_value, .result.result_type = (a_value operation_value b_value) }

#define TEST_CASE_CAST(function_name, op_type, result_type, a_value) \
{ .name = #function_name, .a_str = #a_value, .b_str = "--", .op_str = " cast ", .operation = function_name, .a.op_type = a_value, .b.op_int = 0, .result.result_type = (a_value) }


const test_t tests[] = {
    TEST_CASE(cmp_f, op_f, op_int, 1.0f, 1.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 1234.567f, 765.4321f, ==),
    TEST_CASE(cmp_f, op_f, op_int, -123.0f, -768.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 0.0f, -0.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 1.0f, NAN, ==),
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


    TEST_CASE(mul_u64, op_u64, op_u64, 100ULL, 100ULL, *),
    { .name = "mul_u64 100ULL 100ULL", .operation = mul_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL*100ULL) },
    { .name = "div_u64 100ULL 100ULL", .operation = div_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL/100ULL) },
    { .name = "cmp_u64 100ULL 100ULL", .operation = cmp_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL==100ULL) },
    { .name = "sub_u64 100ULL 100ULL", .operation = sub_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL-100ULL) },
    { .name = "add_u64 100ULL 100ULL", .operation = add_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL+100ULL) },
    { .name = "modulus_u64 100ULL 100ULL", .operation = modulus_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL%100ULL) },
    { .name = "greaterthan_u64 100ULL 100ULL", .operation = greaterthan_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL>100ULL) },
    { .name = "greaterthanorequal_u64 100ULL 100ULL", .operation = greaterthanorequal_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL>=100ULL) },
    { .name = "lessthan_u64 100ULL 100ULL", .operation = lessthan_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL<100ULL) },
    { .name = "lessthanorequal_u64 100ULL 100ULL", .operation = lessthanorequal_u64, .a.op_u64 = 100ULL, .b.op_u64 = 100ULL, .result.op_u64 = (100ULL<=100ULL) },


    TEST_CASE(mul_s64, op_s64, op_s64, 100ULL, 100ULL, *),
    { .name = "mul_s64 100 100", .operation = mul_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL*100ULL) },
    { .name = "mul_s64 100 100", .operation = mul_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL*100ULL) },
    { .name = "div_s64 100 100", .operation = div_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL/100ULL) },
    { .name = "cmp_s64 100 100", .operation = cmp_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL==100ULL) },
    { .name = "sub_s64 100 100", .operation = sub_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL-100ULL) },
    { .name = "add_s64 100 100", .operation = add_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL+100ULL) },
    { .name = "modulus_s64 100 100", .operation = modulus_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL%100ULL) },
    { .name = "greaterthan_s64 100 100", .operation = greaterthan_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL>100ULL) },
    { .name = "greaterthanorequal_s64 100 100", .operation = greaterthanorequal_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100, .result.op_s64 = (100ULL>=100ULL) },
    { .name = "lessthan_s64 100 100", .operation = lessthan_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL<100ULL) },
    { .name = "lessthanorequal_s64 100 100", .operation = lessthanorequal_s64, .a.op_s64 = 100ULL, .b.op_s64 = 100ULL, .result.op_s64 = (100ULL<=100ULL) },


    TEST_CASE(multi_f, op_f, op_f, 100.0f, 100.0f, *),
    { .name = "divi_f 100.0f 100.0f", .operation = divi_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_f = (100.0f/100.0f) },
    { .name = "cmpa_f 100.0f 100.0f", .operation = cmpa_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_int = (100.0f==100.0f) },
    { .name = "sub_f 100.0f 100.0f", .operation = sub_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_f = (100.0f-100.0f) },
    { .name = "add_f 100.0f 100.0f", .operation = add_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_f = (100.0f+100.0f) },
    { .name = "greaterthan_f 100.0f 100.0f", .operation = greaterthan_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_f = (100.0f>100.0f) },
    { .name = "greaterthanorequal_f 100.0f 100.0f", .operation = greaterthanorequal_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_int = (100.0f>=100.0f) },
    { .name = "lessthan_f 100.0f 100.0f", .operation = lessthan_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_f = (100.0f<100.0f) },
    { .name = "lessthanorequal_f 100.0f 100.0f", .operation = lessthanorequal_f, .a.op_f = 100.0f, .b.op_f = 100.0f, .result.op_int = (100.0f<=100.0f) },


    TEST_CASE(mul_d, op_d, op_d, 100.0, 100.0, *),
    TEST_CASE(mul_d, op_d, op_d, -0.125, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, 0.125, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, -HUGE_VAL, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VAL, -HUGE_VAL, *),

    { .name = "div_d 100.0 100.0", .operation = div_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0/100.0) },
    { .name = "cmp_d 100.0 100.0", .operation = cmp_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_int = (100.0==100.0) },
    { .name = "sub_d 100.0 100.0", .operation = sub_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0-100.0) },
    { .name = "add_d 100.0 100.0", .operation = add_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0+100.0) },
    { .name = "greaterthan_d 100.0 100.0", .operation = greaterthan_d, .a.op_d = 100, .b.op_d = 100.0, .result.op_d = (100.0>100.0) },
    { .name = "greaterthanorequal_d 100.0 100.0", .operation = greaterthanorequal_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_int = (100.0>=100.0) },
    { .name = "lessthan_d 100.0 100.0", .operation = lessthan_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_d = (100.0<100.0) },
    { .name = "lessthanorequal_d 100.0 100.0", .operation = lessthanorequal_d, .a.op_d = 100.0, .b.op_d = 100.0, .result.op_int = (100.0<=100.0) },

    TEST_CASE_CAST(f_to_int, op_f, op_int, 0.4),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, 0.4),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, 0.4),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, 0.4),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, 0.4),

    { .name = "f_to_int 0.6", .operation = f_to_int, .a.op_f = 0.4, .b.op_f = 0.0 /* not used */, .result.op_int = 0 },
    { .name = "f_to_s64 0.6", .operation = f_to_s64, .a.op_f = 0.4, .b.op_f = 0.0 /* not used */, .result.op_s64 = 0 },
    { .name = "f_to_u64 0.6", .operation = f_to_u64, .a.op_f = 0.4, .b.op_f = 0.0 /* not used */, .result.op_u64 = 0 },
    { .name = "f_to_s32 0.6", .operation = f_to_s32, .a.op_f = 0.4, .b.op_f = 0.0 /* not used */, .result.op_s32 = 0 },
    { .name = "f_to_u32 0.6", .operation = f_to_u32, .a.op_f = 0.4, .b.op_f = 0.0 /* not used */, .result.op_u32 = 0 },


    { .name = "mul_d -0.125 HUGE_VAL", .operation = mul_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
    { .name = "div_d  HUGE_VAL -0.125", .operation = div_d, .a.op_d = HUGE_VAL, .b.op_d = -0.125, .result.op_d = -INFINITY },
    { .name = "cmp_d -0.125 HUGE_VAL", .operation = cmp_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
    { .name = "sub_d -0.125 HUGE_VAL", .operation = sub_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
    { .name = "add_d -0.125 HUGE_VAL", .operation = add_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = INFINITY },
    { .name = "greaterthan_d -0.125 HUGE_VAL", .operation = greaterthan_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
    { .name = "greaterthanorequal_d -0.125 HUGE_VAL", .operation = greaterthanorequal_d, .a.op_d = -0.125, .b.op_d = HUGE_VAL, .result.op_d = -INFINITY },
    { .name = "lessthan_d HUGE_VAL -0.125", .operation = lessthan_d, .a.op_d = HUGE_VAL, .b.op_d = -0.125, .result.op_d = -INFINITY },
    { .name = "lessthanorequal_d  HUGE_VAL -0.125", .operation = lessthanorequal_d, .a.op_d = HUGE_VAL, .b.op_d = -0.125, .result.op_d = -INFINITY },


};



static int exec_test(int i){
    if( tests[i].a_str != 0 ){
        printf("test %s %s%s%s...", tests[i].name, tests[i].a_str, tests[i].op_str, tests[i].b_str); fflush(stdout);
    } else {
        printf("test %s...", tests[i].name); fflush(stdout);
    }
    if( tests[i].operation(tests[i].a, tests[i].b, tests[i].result) == 0 ){
        printf("failed\n");
        return -1;
    } else {
        printf("passed\n");
    }
    return 0;
}

int num_test_execute(u32 o_flags){

    if( o_flags & NUM_TEST_FLAG_INT ){
        if( int_test() < 0 ){
            return -1;
        }
    }
    if( o_flags & NUM_TEST_FLAG_FLOAT ){
        if( float_test() < 0 ){
            return -1;
        }
    }

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

    t.op_u64 = a.op_u64 / b.op_u64;

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
int modulus_u64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_u64 = a.op_u64 % b.op_u64;

    if( t.op_u64 == result.op_u64 ){
        return 1;
    }

    printf("%lld != %lld...", t.op_u64, result.op_u64);
    return 0;

}


/*
     *  Integer Greaterthan
     */
int greaterthan_u64(operand_t a, operand_t b, operand_t result)
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

    if( t.op_int == result.op_int )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_u64, result.op_u64);
    return 0;
}


/*
     *  Integer Greater than or equal
     */
int greaterthanorequal_u64(operand_t a, operand_t b, operand_t result)
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
int lessthan_u64(operand_t a, operand_t b, operand_t result)
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
int lessthanorequal_u64(operand_t a, operand_t b, operand_t result)
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


///s64


/*
         *  Integer Multiplication
         */
int mul_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = a.op_s64 * b.op_s64;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}

/*
             *  Integer Substraction
             */
int sub_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = a.op_s64 - b.op_s64;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


/*
             *  Integer Addition
             */
int add_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = a.op_s64 + b.op_s64;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}



/*
             *  Integer Division
             */
int div_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = a.op_s64 / b.op_s64;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_u64, result.op_u64);
    return 0;
}



/*
             *  Integer Comparision
             */
int cmp_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    if( a.op_s64 == b.op_s64 )
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

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


/*
             *  Integer modulus
             */
int modulus_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_s64 = a.op_s64 % b.op_s64;

    if( t.op_s64 == result.op_s64 ){
        return 1;
    }

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


/*
             *  Integer Greaterthan
             */
int greaterthan_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    if(a.op_s64 > b.op_s64)
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

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


/*
             *  Integer Greater than or equal
             */
int greaterthanorequal_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    if(a.op_s64 >= b.op_s64)
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

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


int lessthan_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    if(a.op_s64 < b.op_s64)
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


    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


int lessthanorequal_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    if(a.op_s64 <= b.op_s64)
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

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


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
int greaterthan_f(operand_t a, operand_t b, operand_t result)
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
int greaterthanorequal_f(operand_t a, operand_t b, operand_t result)
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
int lessthan_f(operand_t a, operand_t b, operand_t result)
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
int lessthanorequal_f(operand_t a, operand_t b, operand_t result)
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
int greaterthan_d(operand_t a, operand_t b, operand_t result)
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
int greaterthanorequal_d(operand_t a, operand_t b, operand_t result)
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
int lessthan_d(operand_t a, operand_t b, operand_t result)
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
int lessthanorequal_d(operand_t a, operand_t b, operand_t result)
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

    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}

//		float to u64
int f_to_u64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_u64 = (int)a.op_f;

    if( t.op_u64 == result.op_u64 )
    {
        return 1;
    }

    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}


//		float to s64

int f_to_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = (int)a.op_f;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}


//		float to s32

int f_to_s32(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s32 = (int)a.op_f;

    if( t.op_s32 == result.op_s32 )
    {
        return 1;
    }

    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}


//		float to u32
int f_to_u32(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_u32 = (int)a.op_f;

    if( t.op_u32 == result.op_u32 )
    {
        return 1;
    }

    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}
