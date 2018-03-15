


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
    TEST_CASE(cmp_f, op_f, op_int, NAN, 1.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, NAN, NAN, ==),
    TEST_CASE(cmp_f, op_f, op_int, INFINITY, 1.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 0.0f, INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, 0.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 0.0f, -INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, INFINITY, INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, -INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, INFINITY, -INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, INFINITY, ==),
    TEST_CASE(cmp_f, op_f, op_int, 0.0f, HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, 0.0f, -HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, 0.0f, NAN, ==),
    TEST_CASE(cmp_f, op_f, op_int, -0.0f, HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, -0.0f, -HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, -0.0f, NAN, ==),
    TEST_CASE(cmp_f, op_f, op_int, NAN, HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, NAN, -HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, NAN, 3.14159f, ==),
    TEST_CASE(cmp_f, op_f, op_int, NAN, 3.14159f, ==),
    TEST_CASE(cmp_f, op_f, op_int, INFINITY, NAN, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, NAN, ==),
    TEST_CASE(cmp_f, op_f, op_int, INFINITY, HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, INFINITY, -HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, -INFINITY, -HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, 86400.0f, -HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, 86400.0f, HUGE_VAL, ==),
    TEST_CASE(cmp_f, op_f, op_int, -86400.0f, HUGE_VAL, ==),

    TEST_CASE(mul_f, op_f, op_f, 1.0f, HUGE_VALF, *),
    TEST_CASE(mul_f, op_f, op_f, -1.0f, HUGE_VALF, *),
    TEST_CASE(mul_f, op_f, op_f, 1.0f, -HUGE_VALF, *),
    TEST_CASE(mul_f, op_f, op_f, HUGE_VALF, INFINITY, *),
    TEST_CASE(mul_f, op_f, op_f, HUGE_VALF, -INFINITY, *),
    TEST_CASE(mul_f, op_f, op_f, -HUGE_VALF, -INFINITY, *),
    TEST_CASE(mul_f, op_f, op_f, -HUGE_VALF, INFINITY, *),
    TEST_CASE(mul_f, op_f, op_f, -2.25, HUGE_VALF , *),
    TEST_CASE(mul_f, op_f, op_f, HUGE_VALF, 12 , *),
    TEST_CASE(mul_f, op_f, op_f, 0.5f, 10.0f, *),
    TEST_CASE(mul_f, op_f, op_f, -0.5f, -2.0f, *),
    TEST_CASE(mul_f, op_f, op_f, HUGE_VALF, 0.25f, *),
    TEST_CASE(mul_f, op_f, op_f, -0.125f, HUGE_VALF, *),
    TEST_CASE(mul_f, op_f, op_f, 0.0f, -0.0f, *),
    TEST_CASE(mul_f, op_f, op_f, 0.0f, 1440.0f, *),
    TEST_CASE(mul_f, op_f, op_f, -0.0f, 1440.0f, *),
    TEST_CASE(mul_f, op_f, op_f, -1440.0f, 0.0f, *),
    TEST_CASE(mul_f, op_f, op_f, 3.14159f, INFINITY, *),
    TEST_CASE(mul_f, op_f, op_f, 3.14159f, -INFINITY, *),

    TEST_CASE(div_f, op_f, op_f, 0.0f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 1.0f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, -1.0f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 1.0f, -HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, -2.25, HUGE_VALF , /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, -1.0f, /),
    TEST_CASE(div_f, op_f, op_f, -HUGE_VALF, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, -2.25, /),
    TEST_CASE(div_f, op_f, op_f, 0.5f, 10.0f, /),
    TEST_CASE(div_f, op_f, op_f, -0.5f, -2.0f, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, 0.25f, /),
    TEST_CASE(div_f, op_f, op_f, -0.125f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 1.0f, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, -INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, 3.14159f, /),

    TEST_CASE(mul_u64, op_u64, op_u64, 100ULL, 100ULL, *),
    TEST_CASE(div_u64, op_u64, op_u64, 100ULL, 100ULL, /),
    TEST_CASE(cmp_u64, op_u64, op_u64, 100ULL, 100ULL, ==),
    TEST_CASE(sub_u64, op_u64, op_u64, 100ULL, 100ULL, -),
    TEST_CASE(add_u64, op_u64, op_u64, 100ULL, 100ULL, +),
    TEST_CASE(modulus_u64, op_u64, op_u64, 100ULL, 100ULL, %),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 100ULL, 100ULL, >),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 100ULL, 100ULL, >=),
    TEST_CASE(lessthan_u64, op_u64, op_int, 100ULL, 100ULL, <),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 100ULL, 100ULL, <=),

	TEST_CASE(mul_s64, op_s64, op_s64, 100ULL, 100ULL, *),
    TEST_CASE(div_s64, op_s64, op_s64, 100ULL, 100ULL, /),
    TEST_CASE(cmp_s64, op_s64, op_s64, 100ULL, 100ULL, ==),
    TEST_CASE(sub_s64, op_s64, op_s64, 100ULL, 100ULL, -),
    TEST_CASE(add_s64, op_s64, op_s64, 100ULL, 100ULL, +),
    TEST_CASE(modulus_s64, op_s64, op_s64, 100ULL, 100ULL, %),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 100ULL, 100ULL, >),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 100ULL, 100ULL, >=),
    TEST_CASE(lessthan_s64, op_s64, op_int, 100ULL, 100ULL, <),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 100ULL, 100ULL, <=),

    TEST_CASE(multi_f, op_f, op_f, 100.0f, 100.0f, *),
    TEST_CASE(divi_f, op_f, op_f, 100.0f, 100.0f, /),
    TEST_CASE(cmpa_f, op_f, op_int, 100.0f, 100.0f, ==),
    TEST_CASE(sub_f, op_f, op_f, 100.0f, 100.0f, -),
    TEST_CASE(add_f, op_f, op_f, 100.0f, 100.0f, +),
    TEST_CASE(greaterthan_f, op_f, op_int, 100.0f, 100.0f, >),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, 100.0f, 100.0f, >=),
    TEST_CASE(lessthan_f, op_f, op_int, 100.0f, 100.0f, <),
    TEST_CASE(lessthanorequal_f, op_f, op_int, 100.0f, 100.0f, <=),

    TEST_CASE(mul_d, op_d, op_d, 100.0, 100.0, *),
    TEST_CASE(mul_d, op_d, op_d, -0.125, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, 0.125, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, -HUGE_VAL, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VAL, -HUGE_VAL, *),
    TEST_CASE(div_d, op_d, op_d, 100.0, 100.0, /),
    TEST_CASE(cmp_d, op_d, op_int, 100.0, 100.0, ==),
    TEST_CASE(sub_d, op_d, op_d, 100.0, 100.0, -),
    TEST_CASE(add_d, op_d, op_d, 100.0, 100.0, +),
    TEST_CASE(greaterthan_d, op_d, op_int, 100.0, 100.0, >),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, 100.0, 100.0, >=),
    TEST_CASE(lessthan_d, op_d, op_int, 100.0, 100.0, <),
    TEST_CASE(lessthanorequal_d, op_d, op_int, 100.0, 100.0, <=),

    TEST_CASE_CAST(f_to_int, op_f, op_int, 0.4),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, 0.4),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, 0.4),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, 0.4),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, 0.4),
    TEST_CASE_CAST(f_to_int, op_f, op_int, 0.4),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, 0.4),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, 0.4),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, 0.4),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, 0.4),

    TEST_CASE(mul_d, op_d, op_d, -0.125, HUGE_VAL, *),
    TEST_CASE(div_d, op_d, op_d, HUGE_VAL, -0.125, /),
    TEST_CASE(cmp_d, op_d, op_d, -0.125, HUGE_VAL, ==),
    TEST_CASE(sub_d, op_d, op_d, -0.125, HUGE_VAL, -),
    TEST_CASE(add_d, op_d, op_d, -0.125, HUGE_VAL, +),
    TEST_CASE(greaterthan_d, op_d, op_int, -0.125, HUGE_VAL, >),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, -0.125, HUGE_VAL, >=),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, -0.125, <),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, -0.125, <=),

    TEST_CASE(greaterthan_f, op_f, op_int, HUGE_VAL, INFINITY, >),
    TEST_CASE(greaterthan_f, op_f, op_int, INFINITY, HUGE_VAL, >),
    TEST_CASE(greaterthan_f, op_f, op_int, INFINITY, NAN, >),
    TEST_CASE(greaterthan_f, op_f, op_int, NAN, INFINITY, >),
    TEST_CASE(greaterthan_f, op_f, op_int, HUGE_VAL, NAN, >),
    TEST_CASE(greaterthan_f, op_f, op_int, NAN, HUGE_VAL, >),
    TEST_CASE(greaterthan_f, op_f, op_int, HUGE_VAL, HUGE_VAL, >),
    TEST_CASE(greaterthan_f, op_f, op_int, NAN, NAN, >),
    TEST_CASE(greaterthan_f, op_f, op_int, INFINITY, INFINITY, >),
    TEST_CASE(greaterthan_f, op_f, op_int, NAN, 0.0f, >),
    TEST_CASE(greaterthan_f, op_f, op_int, HUGE_VAL, 0.0f, >),
    TEST_CASE(greaterthan_f, op_f, op_int, INFINITY, 0.0f, >),
    TEST_CASE(greaterthan_f, op_f, op_int, 0.0f, NAN, >),
    TEST_CASE(greaterthan_f, op_f, op_int, 0.0f, HUGE_VAL, >),
    TEST_CASE(greaterthan_f, op_f, op_int, 0.0f, INFINITY, >),
    TEST_CASE(greaterthan_f, op_f, op_int, 0.0f, 0.0f, >),

    TEST_CASE(greaterthanorequal_f, op_f, op_int, HUGE_VAL, INFINITY, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, INFINITY, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, INFINITY, NAN, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, NAN, INFINITY, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, HUGE_VAL, NAN, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, NAN, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, HUGE_VAL, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, NAN, NAN, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, INFINITY, INFINITY, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, NAN, 0.0f, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, HUGE_VAL, 0.0f, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, INFINITY, 0.0f, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, 0.0f, NAN, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, 0.0f, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, 0.0f, INFINITY, >=),
    TEST_CASE(greaterthanorequal_f, op_f, op_int, 0.0f, 0.0f, >=),

    TEST_CASE(lessthan_f, op_f, op_int, HUGE_VAL, INFINITY, <),
    TEST_CASE(lessthan_f, op_f, op_int, INFINITY, HUGE_VAL, <),
    TEST_CASE(lessthan_f, op_f, op_int, INFINITY, NAN, <),
    TEST_CASE(lessthan_f, op_f, op_int, NAN, INFINITY, <),
    TEST_CASE(lessthan_f, op_f, op_int, HUGE_VAL, NAN, <),
    TEST_CASE(lessthan_f, op_f, op_int, NAN, HUGE_VAL, <),
    TEST_CASE(lessthan_f, op_f, op_int, HUGE_VAL, HUGE_VAL, <),
    TEST_CASE(lessthan_f, op_f, op_int, NAN, NAN, <),
    TEST_CASE(lessthan_f, op_f, op_int, INFINITY, INFINITY, <),
    TEST_CASE(lessthan_f, op_f, op_int, NAN, 0.0f, <),
    TEST_CASE(lessthan_f, op_f, op_int, HUGE_VAL, 0.0f, <),
    TEST_CASE(lessthan_f, op_f, op_int, INFINITY, 0.0f, <),
    TEST_CASE(lessthan_f, op_f, op_int, 0.0f, NAN, <),
    TEST_CASE(lessthan_f, op_f, op_int, 0.0f, HUGE_VAL, <),
    TEST_CASE(lessthan_f, op_f, op_int, 0.0f, INFINITY, <),
    TEST_CASE(lessthan_f, op_f, op_int, 0.0f, 0.0f, <),

    TEST_CASE(lessthanorequal_f, op_f, op_int, HUGE_VAL, INFINITY, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, INFINITY, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, INFINITY, NAN, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, NAN, INFINITY, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, HUGE_VAL, NAN, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, NAN, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, HUGE_VAL, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, NAN, NAN, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, INFINITY, INFINITY, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, NAN, 0.0f, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, HUGE_VAL, 0.0f, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, INFINITY, 0.0f, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, 0.0f, NAN, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, 0.0f, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, 0.0f, INFINITY, <=),
    TEST_CASE(lessthanorequal_f, op_f, op_int, 0.0f, 0.0f, <=),
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
