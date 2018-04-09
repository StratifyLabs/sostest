


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



// u64
static int mul_u64(operand_t a, operand_t b, operand_t result);
static int div_u64(operand_t a, operand_t b, operand_t result);
static int cmp_u64(operand_t a, operand_t b, operand_t result);
static int sub_u64(operand_t a, operand_t b, operand_t result);
static int add_u64(operand_t a, operand_t b, operand_t result);
static int shift_to_left_u64(operand_t a, operand_t b, operand_t result);
static int shift_to_right_u64(operand_t a, operand_t b, operand_t result);

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
static int mul_f(operand_t a, operand_t b, operand_t result);
static int div_f(operand_t a, operand_t b, operand_t result);
static int cmp_f(operand_t a, operand_t b, operand_t result);
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
static int int_to_f(operand_t a, operand_t b, operand_t result);
static int u64_to_f(operand_t a, operand_t b, operand_t result);
static int s64_to_f(operand_t a, operand_t b, operand_t result);
static int s32_to_f(operand_t a, operand_t b, operand_t result);
static int u32_to_f (operand_t a, operand_t b, operand_t result);

//double to int
static int d_to_int(operand_t a, operand_t b, operand_t result);
static int d_to_u64(operand_t a, operand_t b, operand_t result);
static int d_to_s64(operand_t a, operand_t b, operand_t result);
static int d_to_s32(operand_t a, operand_t b, operand_t result);
static int d_to_u32 (operand_t a, operand_t b, operand_t result);

//int to double
static int int_to_d(operand_t a, operand_t b, operand_t result);
static int u64_to_d(operand_t a, operand_t b, operand_t result);
static int s64_to_d(operand_t a, operand_t b, operand_t result);
static int s32_to_d(operand_t a, operand_t b, operand_t result);
static int u32_to_d (operand_t a, operand_t b, operand_t result);


#define TEST_CASE(function_name, op_type, result_type, a_value, b_value, operation_value) \
{ .name = #function_name, .a_str = #a_value, .b_str = #b_value, .op_str = #operation_value, .operation = function_name, .a.op_type = a_value, .b.op_type = b_value, .result.result_type = (a_value operation_value b_value) }

#define TEST_CASE_CAST(function_name, op_type, result_type, a_value) \
{ .name = #function_name, .a_str = #a_value, .b_str = "--", .op_str = " cast ", .operation = function_name, .a.op_type = a_value, .b.op_int = 0, .result.result_type = (a_value) }


const test_t tests[] = {
    TEST_CASE(mul_u64, op_u64, op_u64, 100ULL, 100ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 9999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 99999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 999999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 9999999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 99999999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 99999999ULL, -888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 999999999ULL, 888ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 999999999ULL, 999999ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 999999999ULL, 9999999ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 999983ULL, 999983ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 67ULL, 67ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 9223372036854775807ULL, 1ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 9223372036854775807ULL, 2ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 9223372036854775807ULL, 3ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 4611686018427387903ULL, 4ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 2305843009213693951ULL, 8ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 2305843009213693951ULL, 0ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 0x0fffffffffffffffULL, 2ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 0x0000ffff1fffffffULL, 12ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 0x00fffffffffffffeULL, 3ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 0x00000fffffffffffULL, 8ULL, *),
    TEST_CASE(mul_u64, op_u64, op_u64, 0x00000fffffffffffULL, 0x00000000ffULL, *),

    TEST_CASE(div_u64, op_u64, op_u64, 100ULL, 100ULL, /),//failed
    TEST_CASE(div_u64, op_u64, op_u64, 17ULL, 1ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 1ULL, 10ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 41ULL, 1ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 1000000000ULL, 10ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 37ULL, 1000000000ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 1000000000ULL, 4000000000ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 998681ULL, 999983ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 998681ULL, 17ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 999491999491ULL, 11ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 11ULL, 999491999491ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 11ULL, 18446744073709551614ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 1ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 2ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 4ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 8ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 11ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 16ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 9223372036854775807ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 4611686018427387903ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 18446744073709551614ULL, 2305843009213693951ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 998831ULL, 1ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 0xfffffeffffffffffULL, 2ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 0xfffffefffffffffeULL, 2ULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 0xffffffffffffffffULL, 0xffffffffffffffffULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 0xfffffffffffffffeULL, 0xfffffffffffffffeULL, /),
    TEST_CASE(div_u64, op_u64, op_u64, 0x8000000000000000ULL, 0x8000000000000000ULL, /),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0x0000000000000001ULL, 17ULL, <<),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0x0000000000000001ULL, 33ULL, <<),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0x1000000000000001ULL, 33ULL, <<),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0xffffffffffffffffULL, 12ULL, <<),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0xeeeeeeeeeeeeeeeeULL, 28ULL, <<),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0x8888888888888888ULL, 31ULL, <<),
    TEST_CASE(shift_to_left_u64, op_u64, op_u64, 0x0000000080000000ULL, 31ULL, <<),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0x0000000000000001ULL, 17ULL, >>),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0x0000000000000001ULL, 33ULL, >>),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0x1000000000000001ULL, 33ULL, >>),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0xffffffffffffffffULL, 12ULL, >>),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0xeeeeeeeeeeeeeeeeULL, 28ULL, >>),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0x8888888888888888ULL, 31ULL, >>),
    TEST_CASE(shift_to_right_u64, op_u64, op_u64, 0x0000000080000000ULL, 31ULL, >>),



    TEST_CASE(cmp_u64, op_u64, op_u64, 100ULL, 100ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 18446744073709551614ULL, 18446744073709551615ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 18446744073709551614ULL, 0ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 1ULL, 18446744073709551615ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 999491999491ULL, 18446744073709551615ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 0ULL, 0ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 1ULL, 1ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 1ULL, 0ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 18446744073709551615ULL, 18446744073709551615ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 18446744073709551615ULL, 0ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 18446744073709551615ULL, 9223372036854775807ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 9223372036854775807ULL, 18446744073709551615ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 9223372036854775807ULL, 0ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 1ULL, 9223372036854775807ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 4611686018427387903ULL, 9223372036854775807ULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 0xffffffffffffffffULL, 0xffffffffffffffffULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 0xfffffffffffffffeULL, 0xfffffffffffffffeULL, ==),
    TEST_CASE(cmp_u64, op_u64, op_u64, 0x8000000000000000ULL, 0x8000000000000000ULL, ==),

    TEST_CASE(sub_u64, op_u64, op_u64, 100ULL, 100ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 18446744073709551614ULL, 18446744073709551615ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 18446744073709551615ULL, 18446744073709551614ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 18446744073709551615ULL, 9223372036854775807ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 18446744073709551614ULL, 0ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 0ULL, 0ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 1ULL, 0ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 1ULL, 1ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 0ULL, 18446744073709551614ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 18446744073709551614ULL, 0ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 18446744073709551614ULL, 18446744073709551614ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 999377ULL, 999023ULL, -),
    TEST_CASE(sub_u64, op_u64, op_u64, 999359ULL, 999983ULL, -),

    TEST_CASE(add_u64, op_u64, op_u64, 100ULL, 100ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 0ULL, 0ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 0ULL, 1ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 1ULL, 1ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 1ULL, 12ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 9223372036854775807ULL, 9223372036854775807ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 18446744073709551614ULL, 0ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 18446744073709551614ULL, 1ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 18446744073709551614ULL, 9223372036854775807ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 0ULL, 9223372036854775807ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 1ULL, 9223372036854775807ULL, +),
    TEST_CASE(add_u64, op_u64, op_u64, 2305843009213693951ULL, 9223372036854775807ULL, +),

    TEST_CASE(modulus_u64, op_u64, op_u64, 100ULL, 100ULL, %),//failed
    TEST_CASE(modulus_u64, op_u64, op_u64, 1ULL, 1ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 100ULL, 1ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 10ULL, 100ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 17ULL, 1ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 1ULL, 10ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 41ULL, 1ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 1000000000ULL, 10ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 37ULL, 1000000000ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 1000000000ULL, 4000000000ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 998681ULL, 999983ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 998681ULL, 17ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 999491999491ULL, 11ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 11ULL, 999491999491ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 11ULL, 18446744073709551614ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 1ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 2ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 4ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 8ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 11ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 16ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 998831ULL, 1ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 9223372036854775807ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 4611686018427387903ULL, %),
    TEST_CASE(modulus_u64, op_u64, op_u64, 18446744073709551614ULL, 2305843009213693951ULL, %),

    TEST_CASE(greaterthan_u64, op_u64, op_int, 100ULL, 100ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 100ULL, 1ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 0ULL, 100ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 1ULL, 0ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 18446744073709551614ULL, 2305843009213693951ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 2305843009213693951ULL, 18446744073709551614ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 18446744073709551614ULL, 18446744073709551614ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 2305843009213693951ULL, 2305843009213693951ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 18446744073709551614ULL, 0ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 18446744073709551614ULL, 1ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 1ULL, 18446744073709551614ULL, >),
    TEST_CASE(greaterthan_u64, op_u64, op_int, 0ULL, 18446744073709551614ULL, >),

    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 100ULL, 100ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 100ULL, 1ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 0ULL, 100ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 1ULL, 0ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 2305843009213693951ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 2305843009213693951ULL, 18446744073709551614ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 18446744073709551614ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 2305843009213693951ULL, 2305843009213693951ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 0ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 1ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 1ULL, 18446744073709551614ULL, >=),
    TEST_CASE(greaterthanorequal_u64, op_u64, op_int, 0ULL, 18446744073709551614ULL, >=),

    TEST_CASE(lessthan_u64, op_u64, op_int, 100ULL, 100ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 100ULL, 1ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 0ULL, 100ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 1ULL, 0ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 18446744073709551614ULL, 2305843009213693951ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 2305843009213693951ULL, 18446744073709551614ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 18446744073709551614ULL, 18446744073709551614ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 2305843009213693951ULL, 2305843009213693951ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 18446744073709551614ULL, 0ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 18446744073709551614ULL, 1ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 1ULL, 18446744073709551614ULL, <),
    TEST_CASE(lessthan_u64, op_u64, op_int, 0ULL, 18446744073709551614ULL, <),

    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 100ULL, 100ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 100ULL, 1ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 0ULL, 100ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 1ULL, 0ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 2305843009213693951ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 2305843009213693951ULL, 18446744073709551614ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 18446744073709551614ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 2305843009213693951ULL, 2305843009213693951ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 0ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 18446744073709551614ULL, 1ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 1ULL, 18446744073709551614ULL, <=),
    TEST_CASE(lessthanorequal_u64, op_u64, op_int, 0ULL, 18446744073709551614ULL, <=),

    TEST_CASE(mul_s64, op_s64, op_s64, 100LL, 100LL, *),//failed
    TEST_CASE(mul_s64, op_s64, op_s64, 999LL, 888LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 9999LL, 888LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 9999999LL, 888LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 99999999LL, 888LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 99999999LL, -888LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 999999999LL, 999999LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 999999999LL, 9999999LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -999999999LL, 9999999LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 999983LL, 999983LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 67LL, 67LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -67LL, -67LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 9223372036854775807LL, 1LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 3223372036854775806LL, 2LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 4611686018427387903LL, 2LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 2305843009213693951LL, 4LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, 2305843009213693951LL, 0LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -9223372036854775807LL, 1LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -3223372036854775807LL, 2LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -4611686018427387903LL, 2LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -2305843009213693951LL, 4LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -2305843009213693951LL, 0LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -9223372036854775807LL, -1LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -3223372036854775807LL, -2LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -4611686018427387903LL, -2LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -2305843009213693951LL, -4LL, *),
    TEST_CASE(mul_s64, op_s64, op_s64, -2305843009213693951LL, -0LL, *),

    TEST_CASE(div_s64, op_s64, op_s64, 100LL, 100LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 17LL, 1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 1LL, 10LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 41LL, 1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 1000000000LL, 10LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 37LL, 1000000000LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 1000000000LL, 4000000000LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 998681LL, 999983LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 998681LL, 17LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 999491999491LL, 11LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 11LL, 999491999491LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 11LL, 8446744073709551614LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 7446744073709551614LL, 1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 2LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 4LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 8LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 11LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 16LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 9223372036854775807LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 4611686018427387903LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, 2305843009213693951LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 998831LL, 1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 998831LL, -999809LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -998917LL, -999809LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -17LL, 1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 1LL, 10LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -41LL, -1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -1000000000LL, 10LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -37LL, 1000000000LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -1000000000LL, 4000000000LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -998681LL, 999983LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -998681LL, 17LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 999491999491LL, -11LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -11LL, -999491999491LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 0LL, 999491999491LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 0LL, -999491999491LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -11LL, 8446744073709551614LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -8446744073709551614LL, 1LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -9223372036854775807LL, 2LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, -4LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -9223372036854775807LL, 8LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, -11LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -9223372036854775807LL, 16LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, -9223372036854775807LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, -9223372036854775807LL, 4611686018427387903LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 9223372036854775807LL, -2305843009213693951LL, /),
    TEST_CASE(div_s64, op_s64, op_s64, 0LL, -2305843009213693951LL, /),

    TEST_CASE(cmp_s64, op_s64, op_s64, 100LL, 100LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, 9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 1LL, 9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 999491999491LL, 9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 0LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 1LL, 1LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 1LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, 9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, 4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 4611686018427387903LL, 4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 4611686018427387903LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 1LL, 4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 2305843009213693951LL, 4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 2305843009213693951LL, 2305843009213693951LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -9223372036854775807LL, 9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -9223372036854775807LL, -9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, -9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -9223372036854775807LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -1LL, 9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 999491999491LL, -9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 1LL, -1LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -1LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 9223372036854775807LL, -9223372036854775807LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -9223372036854775807LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -9223372036854775807LL, 4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 4611686018427387903LL, -4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -4611686018427387903LL, 0LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 1LL, -4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, -2305843009213693951LL, 4611686018427387903LL, ==),
    TEST_CASE(cmp_s64, op_s64, op_s64, 2305843009213693951LL, -2305843009213693951LL, ==),

    TEST_CASE(sub_s64, op_s64, op_s64, 100LL, 100LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 0LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 1LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 1LL, 1LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 9223372036854775807LL, 9223372036854775807LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 9223372036854775807LL, 2305843009213693951LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 9223372036854775807LL, 4611686018427387903LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 9223372036854775807LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 0LL, 9223372036854775807LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 9223372036854775807LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 9223372036854775807LL, 9223372036854775807LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 999377LL, 999023LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 999359LL, 999983LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, -1LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, -1LL, 1LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 4223372036854775807LL, -4223372036854775807LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, -4223372036854775807LL, 2305843009213693951LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 4223372036854775807LL, -4611686018427387903LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, -9223372036854775807LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 0LL, -9223372036854775807LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, -9223372036854775807LL, 0LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 4611686018427387903LL, 9223372036854775807LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, 999377LL, -999023LL, -),
    TEST_CASE(sub_s64, op_s64, op_s64, -999359LL, 999983LL, -),

    TEST_CASE(add_s64, op_s64, op_s64, 100LL, 100LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 0LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 1LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 1LL, 1LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 4223372036854775807LL, 4223372036854775807LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 6223372036854775807LL, 2305843009213693951LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 4223372036854775807LL, 4611686018427387903LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 9223372036854775807LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 0LL, 9223372036854775807LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 9223372036854775807LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 1223372036854775807LL, 7223372036854775807LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 999377LL, 999023LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 999359LL, 999983LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -1LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -1LL, 1LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 9223372036854775807LL, -9223372036854775807LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -9223372036854775807LL, 2305843009213693951LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 9223372036854775807LL, -4611686018427387903LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -9223372036854775807LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 0LL, -9223372036854775807LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -9223372036854775807LL, 0LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 4611686018427387903LL, 4223372036854775807LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -4611686018427387903LL, -4611686018427387903LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, 999377LL, -999023LL, +),
    TEST_CASE(add_s64, op_s64, op_s64, -999359LL, 999983LL, +),

    TEST_CASE(modulus_s64, op_s64, op_s64, 100LL, 100LL, %),//failed
    TEST_CASE(modulus_s64, op_s64, op_s64, 17LL, 1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 1LL, 10LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 41LL, 1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 1000000000LL, 10LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 37LL, 1000000000LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 1000000000LL, 4000000000LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 998681LL, 999983LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 998681LL, 17LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 999491999491LL, 11LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 11LL, 999491999491LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 11LL, 9146744073709551614LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9146744073709551614LL, 1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, 2LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, 4LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, 8LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, 11LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, 16LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775607LL, 9223372036854775807LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, 4611686018424387903LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036824775807LL, 2305843009213693951LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 998831LL, 1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 998831LL, -999809LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -998917LL, -999809LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -17LL, 1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 1LL, 10LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -41LL, -1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -1000000000LL, 10LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -37LL, 1000000000LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -1000000000LL, 4000000000LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -998681LL, 999983LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -998681LL, 17LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 999491999491LL, -11LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -11LL, -999491999491LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 0LL, 999491999491LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 0LL, -999491999491LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -11LL, 9146744073709551614LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9146744073709551614LL, 1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 2LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, -4LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 8LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, -11LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 16LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036834775807LL, -9223372036854775807LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 922337203LL, 775807LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036834775807LL, -54775807LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 4611686018427387903LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 84273LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 903LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, -9223372036854775807LL, 3LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 9223372036854775807LL, -2305843009213693951LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 2305843009213693951LL, -230LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 2305843009213693951LL, -3LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 2305843009213693951LL, -1LL, %),
    TEST_CASE(modulus_s64, op_s64, op_s64, 0LL, -2305843009213693951LL, %),

    TEST_CASE(greaterthan_s64, op_s64, op_int, 100LL, 100LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 100LL, 1LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 0LL, 100LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 1LL, 0LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 9223372036854775807LL, 9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 2305843009213693951LL, 922337203685475807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 922337203685477507LL, 9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 2305843009213693951LL, 2305843009213693951LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 9223372036854775807LL, 0LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 9223372036854775807LL, 1LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 1LL, 9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 0LL, 9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -100LL, 1LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 0LL, -100LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -1LL, -0LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -2305843009213693951LL, 9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 9223372036854775807LL, -9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 922337203854775807LL, -922337203685475807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -9223372036854775807LL, 0LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -9223372036854775807LL, 1LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -9223372036854775807LL, -1LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, -1LL, -9223372036854775807LL, >),
    TEST_CASE(greaterthan_s64, op_s64, op_int, 0LL, -9223372036854775807LL, >),

    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 100LL, 100LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 100LL, 1LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 0LL, 100LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 1LL, 0LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 9223372036854775807LL, 9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 2305843009213693951LL, 922337203685475807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 922337203685477507LL, 9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 2305843009213693951LL, 2305843009213693951LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 9223372036854775807LL, 0LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 9223372036854775807LL, 1LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 1LL, 9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 0LL, 9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -100LL, 1LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 0LL, -100LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -1LL, -0LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -2305843009213693951LL, 9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 9223372036854775807LL, -9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 922337203854775807LL, -922337203685475807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -9223372036854775807LL, 0LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -9223372036854775807LL, 1LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -9223372036854775807LL, -1LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, -1LL, -9223372036854775807LL, >=),
    TEST_CASE(greaterthanorequal_s64, op_s64, op_int, 0LL, -9223372036854775807LL, >=),

    TEST_CASE(lessthan_s64, op_s64, op_int, 100LL, 100LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 100LL, 1LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 0LL, 100LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 1LL, 0LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 9223372036854775807LL, 9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 2305843009213693951LL, 922337203685475807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 922337203685477507LL, 9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 2305843009213693951LL, 2305843009213693951LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 9223372036854775807LL, 0LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 9223372036854775807LL, 1LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 1LL, 9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 0LL, 9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -100LL, 1LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 0LL, -100LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -1LL, -0LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -2305843009213693951LL, 9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 9223372036854775807LL, -9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 922337203854775807LL, -922337203685475807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -9223372036854775807LL, 0LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -9223372036854775807LL, 1LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -9223372036854775807LL, -1LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, -1LL, -9223372036854775807LL, <),
    TEST_CASE(lessthan_s64, op_s64, op_int, 0LL, -9223372036854775807LL, <),

    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 100LL, 100LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 100LL, 1LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 0LL, 100LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 1LL, 0LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 9223372036854775807LL, 9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 2305843009213693951LL, 922337203685475807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 922337203685477507LL, 9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 2305843009213693951LL, 2305843009213693951LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 9223372036854775807LL, 0LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 9223372036854775807LL, 1LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 1LL, 9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 0LL, 9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -100LL, 1LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 0LL, -100LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -1LL, -0LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -2305843009213693951LL, 9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 9223372036854775807LL, -9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 922337203854775807LL, -922337203685475807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -9223372036854775807LL, -2305843009213693951LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -9223372036854775807LL, 0LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -9223372036854775807LL, 1LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -9223372036854775807LL, -1LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, -1LL, -9223372036854775807LL, <=),
    TEST_CASE(lessthanorequal_s64, op_s64, op_int, 0LL, -9223372036854775807LL, <=),

    TEST_CASE(cmp_f, op_f, op_int, 1.0f, 1.0f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 1234.567f, 765.4321f, ==),
    TEST_CASE(cmp_f, op_f, op_int, 3.1415926535897932384626433832795f, 3.141592653589793238462643383279f, ==),
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
    TEST_CASE(mul_f, op_f, op_f, -2.25f, HUGE_VALF , *),
    TEST_CASE(mul_f, op_f, op_f, HUGE_VALF, 12.0f , *),
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
    TEST_CASE(mul_f, op_f, op_f, 3.14159f, -1.175494e-27f, *),
    TEST_CASE(mul_f, op_f, op_f, 3.14159e+38f, -1.175494e-33f, *),
    TEST_CASE(mul_f, op_f, op_f, -3.14159e+38f, -1.175494e-3f, *),
    TEST_CASE(mul_f, op_f, op_f, 3.14159e-38f, -1.175494e-4f, *),


    TEST_CASE(div_f, op_f, op_f, 0.0f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 0.0f, 8765.8765f, /),
    TEST_CASE(div_f, op_f, op_f, 1.0f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 0.0f, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, -1.0f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 1.0f, -HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, -2.25f, HUGE_VALF , /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, -HUGE_VALF, -INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, -1.0f, /),
    TEST_CASE(div_f, op_f, op_f, -HUGE_VALF, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, -2.25f, /),
    TEST_CASE(div_f, op_f, op_f, 0.5f, 10.0f, /),
    TEST_CASE(div_f, op_f, op_f, -0.5f, -2.0f, /),
    TEST_CASE(div_f, op_f, op_f, HUGE_VALF, 0.25f, /),
    TEST_CASE(div_f, op_f, op_f, -0.125f, HUGE_VALF, /),
    TEST_CASE(div_f, op_f, op_f, 1.0f, 1.0f, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, -INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, 3.14159f, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, -INFINITY, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159f, 3.14159f, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159e+38f, -1.175494e-33f, /),
    TEST_CASE(div_f, op_f, op_f, -3.14159e+38f, -1.175494e-3f, /),
    TEST_CASE(div_f, op_f, op_f, 3.14159e-38f, -1.175494e-4f, /),
    TEST_CASE(div_f, op_f, op_f, 13.14159e+18f, -13.175494e-33f, /),
    TEST_CASE(div_f, op_f, op_f, -133.14159e+8f, -31.175494e-3f, /),
    TEST_CASE(div_f, op_f, op_f, 83.14159e-38f, -1.175494e-4f, /),

    TEST_CASE(sub_f, op_f, op_f, 100.0f, 100.0f, -),
    TEST_CASE(sub_f, op_f, op_f, -0.125f, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, -0.125f, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, 1234.567f, 765.4321f, -),
    TEST_CASE(sub_f, op_f, op_f, 3.1415926535897932384626433832795f, 3.141592653589793238462643383279f, -),
    TEST_CASE(sub_f, op_f, op_f, -123.0f, -768.0f, -),
    TEST_CASE(sub_f, op_f, op_f, 0.0f, -0.0f, -),
    TEST_CASE(sub_f, op_f, op_f, INFINITY, 1.0f, -),
    TEST_CASE(sub_f, op_f, op_f, 0.0f, INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, -INFINITY, 0.0f, -),
    TEST_CASE(sub_f, op_f, op_f, 0.0f, -INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, INFINITY, INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, -INFINITY, -INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, INFINITY, -INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, -INFINITY, INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, -INFINITY, INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, 0.0f, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, 0.0f, -HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, -0.0f, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, -0.0f, -HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, INFINITY, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, -INFINITY, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, INFINITY, -HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, -INFINITY, -HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, 86400.0f, -HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, 86400.0f, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, -86400.0f, HUGE_VAL, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159f, INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159f, -INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159f, 3.14159f, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159f, -INFINITY, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159f, 3.14159f, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159e+28f, -1.175494e-5f, -),
    TEST_CASE(sub_f, op_f, op_f, -3.14159e+18f, -1.175494e-4f, -),
    TEST_CASE(sub_f, op_f, op_f, 3.14159e-18f, -1.175494e-4f, -),
    TEST_CASE(sub_f, op_f, op_f, 13.14159e+18f, -13.175494e-33f, -),
    TEST_CASE(sub_f, op_f, op_f, -133.14159e+11f, -31.175494e-3f, -),
    TEST_CASE(sub_f, op_f, op_f, 83.14159e-28f, -1.175494e-4f, -),

    TEST_CASE(add_f, op_f, op_f, 100.0f, 100.0f, +),
    TEST_CASE(add_f, op_f, op_f, -0.125f, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, 1234.567f, 765.4321f, +),
    TEST_CASE(add_f, op_f, op_f, 3.1415926535897932384626433832795f, 3.141592653589793238462643383279f, +),
    TEST_CASE(add_f, op_f, op_f, -123.0f, -768.0f, +),
    TEST_CASE(add_f, op_f, op_f, 0.0f, -0.0f, +),
    TEST_CASE(add_f, op_f, op_f, INFINITY, 1.0f, +),
    TEST_CASE(add_f, op_f, op_f, 0.0f, INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, -INFINITY, 0.0f, +),
    TEST_CASE(add_f, op_f, op_f, 0.0f, -INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, INFINITY, INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, -INFINITY, -INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, INFINITY, -INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, -INFINITY, INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, -INFINITY, INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, 0.0f, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, 0.0f, -HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, -0.0f, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, -0.0f, -HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, INFINITY, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, -INFINITY, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, INFINITY, -HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, -INFINITY, -HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, 86400.0f, -HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, 86400.0f, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, -86400.0f, HUGE_VAL, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159f, INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159f, -INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159f, 3.14159f, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159f, -INFINITY, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159f, 3.14159f, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159e+28f, -1.175494e-13f, +),
    TEST_CASE(add_f, op_f, op_f, -3.14159e+38f, -1.175494e-23f, +),
    TEST_CASE(add_f, op_f, op_f, 3.14159e-8f, -1.175494e-4f, +),
    TEST_CASE(add_f, op_f, op_f, 13.14159e+18f, -13.175494e-23f, +),
    TEST_CASE(add_f, op_f, op_f, -133.14159e+18f, -31.175494e-3f, +),
    TEST_CASE(add_f, op_f, op_f, 83.14159e-28f, -1.175494e-4f, +),



    TEST_CASE(greaterthan_f, op_f, op_int, 100.0f, 100.0f, >),
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

    TEST_CASE(greaterthanorequal_f, op_f, op_int, 100.0f, 100.0f, >=),
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

    TEST_CASE(lessthan_f, op_f, op_int, 100.0f, 100.0f, <),
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

    TEST_CASE(lessthanorequal_f, op_f, op_int, 100.0f, 100.0f, <=),
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

    TEST_CASE(mul_d, op_d, op_d, 100.0, 100.0, *),
    TEST_CASE(mul_d, op_d, op_d, -0.125, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, 0.125, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, -HUGE_VAL, HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VAL, -HUGE_VAL, *),
    TEST_CASE(mul_d, op_d, op_d, -1.0, HUGE_VALF, *),
    TEST_CASE(mul_d, op_d, op_d, 1.0, -HUGE_VALF, *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VALF, INFINITY, *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VALF, -INFINITY, *),
    TEST_CASE(mul_d, op_d, op_d, -HUGE_VALF, -INFINITY, *),
    TEST_CASE(mul_d, op_d, op_d, -HUGE_VALF, INFINITY, *),
    TEST_CASE(mul_d, op_d, op_d, -2.25, HUGE_VALF , *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VALF, 12 , *),
    TEST_CASE(mul_d, op_d, op_d, 0.5, 10.0, *),
    TEST_CASE(mul_d, op_d, op_d, -0.5, -2.0, *),
    TEST_CASE(mul_d, op_d, op_d, HUGE_VALF, 0.25, *),
    TEST_CASE(mul_d, op_d, op_d, 0.0, -0.0, *),
    TEST_CASE(mul_d, op_d, op_d, 0.0, 1440.0, *),
    TEST_CASE(mul_d, op_d, op_d, -0.0, 1440.0, *),
    TEST_CASE(mul_d, op_d, op_d, -1440.0, 0.0, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159, INFINITY, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159, -INFINITY, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159, -1.175494e-40, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159e+48, -1.175494e-100, *),
    TEST_CASE(mul_d, op_d, op_d, -3.14159e+68, -1.175494e-200, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159e-38, -1.175494e-40, *),
    TEST_CASE(mul_d, op_d, op_d, -3.14159e+48, 1.175494e-100, *),
    TEST_CASE(mul_d, op_d, op_d, -3.14159e+68, 1.175494e-200, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159e-38, -1.175494e+40, *),
    TEST_CASE(mul_d, op_d, op_d, 3.14159e+38, -1.175494e+40, *),

    TEST_CASE(div_d, op_d, op_d, 100.0, 100.0, /),
    TEST_CASE(div_d, op_d, op_d, HUGE_VAL, -0.125, /),
    TEST_CASE(div_d, op_d, op_d, 0.0, 8765.8765, /),
    TEST_CASE(div_d, op_d, op_d, 1.0, HUGE_VALF, /),
    TEST_CASE(div_d, op_d, op_d, 0.0, 1.0, /),
    TEST_CASE(div_d, op_d, op_d, -1.0, HUGE_VALF, /),
    TEST_CASE(div_d, op_d, op_d, 1.0, -HUGE_VALF, /),
    TEST_CASE(div_d, op_d, op_d, -2.25, HUGE_VALF , /),
    TEST_CASE(div_d, op_d, op_d, HUGE_VALF, INFINITY, /),
    TEST_CASE(div_d, op_d, op_d, -HUGE_VALF, -INFINITY, /),
    TEST_CASE(div_d, op_d, op_d, HUGE_VALF, 1.0, /),
    TEST_CASE(div_d, op_d, op_d, HUGE_VALF, -1.0, /),
    TEST_CASE(div_d, op_d, op_d, -HUGE_VALF, 1.0, /),
    TEST_CASE(div_d, op_d, op_d, HUGE_VALF, -2.25, /),
    TEST_CASE(div_d, op_d, op_d, 0.5, 10.0, /),
    TEST_CASE(div_d, op_d, op_d, -0.5, -2.0, /),
    TEST_CASE(div_d, op_d, op_d, HUGE_VALF, 0.25, /),
    TEST_CASE(div_d, op_d, op_d, -0.125, HUGE_VALF, /),
    TEST_CASE(div_d, op_d, op_d, 1.0, 1.0, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159, INFINITY, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159, -INFINITY, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159, 3.14159, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159, -INFINITY, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159, 3.14159, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159e+78, -1.175494e-53, /),
    TEST_CASE(div_d, op_d, op_d, -3.14159e+78, -1.175494e-43, /),
    TEST_CASE(div_d, op_d, op_d, 3.14159e-88, -1.175494e-40, /),
    TEST_CASE(div_d, op_d, op_d, 13.14159e+18, -13.175494e-233, /),
    TEST_CASE(div_d, op_d, op_d, -133.14159e+118, -31.175494e-3, /),
    TEST_CASE(div_d, op_d, op_d, 83.14159e-288, -1.175494e-4, /),

    TEST_CASE(cmp_d, op_d, op_int, 100.0, 100.0, ==),
    TEST_CASE(cmp_d, op_d, op_int, -0.125, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, 1234.567, 765.4321, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.1415926535897932384626433832795, 3.141592653589793238462643383279, ==),
    TEST_CASE(cmp_d, op_d, op_int, -123.0, -768.0, ==),
    TEST_CASE(cmp_d, op_d, op_int, 0.0, -0.0, ==),
    TEST_CASE(cmp_d, op_d, op_int, 1.0, NAN, ==),
    TEST_CASE(cmp_d, op_d, op_int, NAN, 1.0, ==),
    TEST_CASE(cmp_d, op_d, op_int, NAN, NAN, ==),
    TEST_CASE(cmp_d, op_d, op_int, INFINITY, 1.0, ==),
    TEST_CASE(cmp_d, op_d, op_int, 0.0, INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, 0.0, ==),
    TEST_CASE(cmp_d, op_d, op_int, 0.0, -INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, INFINITY, INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, -INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, INFINITY, -INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, 0.0, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, 0.0, -HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, 0.0, NAN, ==),
    TEST_CASE(cmp_d, op_d, op_int, -0.0, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, -0.0, -HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, -0.0, NAN, ==),
    TEST_CASE(cmp_d, op_d, op_int, NAN, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, NAN, -HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, NAN, 3.14159, ==),
    TEST_CASE(cmp_d, op_d, op_int, NAN, 3.14159, ==),
    TEST_CASE(cmp_d, op_d, op_int, INFINITY, NAN, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, NAN, ==),
    TEST_CASE(cmp_d, op_d, op_int, INFINITY, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, INFINITY, -HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, -INFINITY, -HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, 86400.0, -HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, 86400.0, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, -86400.0, HUGE_VAL, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159, INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159, -INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159, 3.14159, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159, -INFINITY, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159, 3.14159, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159e+78, -1.175494e-53, ==),
    TEST_CASE(cmp_d, op_d, op_int, -3.14159e+78, -1.175494e-43, ==),
    TEST_CASE(cmp_d, op_d, op_int, 3.14159e-88, -1.175494e-40, ==),
    TEST_CASE(cmp_d, op_d, op_int, 13.14159e+18, -13.175494e-233, ==),
    TEST_CASE(cmp_d, op_d, op_int, -133.14159e+118, -31.175494e-3, ==),
    TEST_CASE(cmp_d, op_d, op_int, 83.14159e-288, -1.175494e-4, ==),

    TEST_CASE(sub_d, op_d, op_d, 100.0, 100.0, -),
    TEST_CASE(sub_d, op_d, op_d, -0.125, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, 1234.567, 765.4321, -),
    TEST_CASE(sub_d, op_d, op_d, 3.1415926535897932384626433832795, 3.141592653589793238462643383279, -),
    TEST_CASE(sub_d, op_d, op_d, -123.0, -768.0, -),
    TEST_CASE(sub_d, op_d, op_d, 0.0, -0.0, -),
    TEST_CASE(sub_d, op_d, op_d, INFINITY, 1.0, -),
    TEST_CASE(sub_d, op_d, op_d, 0.0, INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, -INFINITY, 0.0, -),
    TEST_CASE(sub_d, op_d, op_d, 0.0, -INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, INFINITY, INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, -INFINITY, -INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, INFINITY, -INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, -INFINITY, INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, -INFINITY, INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, 0.0, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, 0.0, -HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, -0.0, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, -0.0, -HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, INFINITY, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, -INFINITY, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, INFINITY, -HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, -INFINITY, -HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, 86400.0, -HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, 86400.0, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, -86400.0, HUGE_VAL, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159, INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159, -INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159, 3.14159, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159, -INFINITY, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159, 3.14159, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159e+78, -1.175494e-53, -),
    TEST_CASE(sub_d, op_d, op_d, -3.14159e+78, -1.175494e-43, -),
    TEST_CASE(sub_d, op_d, op_d, 3.14159e-88, -1.175494e-40, -),
    TEST_CASE(sub_d, op_d, op_d, 13.14159e+18, -13.175494e-233, -),
    TEST_CASE(sub_d, op_d, op_d, -133.14159e+118, -31.175494e-3, -),
    TEST_CASE(sub_d, op_d, op_d, 83.14159e-288, -1.175494e-4, -),

    TEST_CASE(add_d, op_d, op_d, 100.0, 100.0, +),
    TEST_CASE(add_d, op_d, op_d, -0.125, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, 1234.567, 765.4321, +),
    TEST_CASE(add_d, op_d, op_d, 3.1415926535897932384626433832795, 3.141592653589793238462643383279, +),
    TEST_CASE(add_d, op_d, op_d, -123.0, -768.0, +),
    TEST_CASE(add_d, op_d, op_d, 0.0, -0.0, +),
    TEST_CASE(add_d, op_d, op_d, INFINITY, 1.0, +),
    TEST_CASE(add_d, op_d, op_d, 0.0, INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, -INFINITY, 0.0, +),
    TEST_CASE(add_d, op_d, op_d, 0.0, -INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, INFINITY, INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, -INFINITY, -INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, INFINITY, -INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, -INFINITY, INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, -INFINITY, INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, 0.0, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, 0.0, -HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, -0.0, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, -0.0, -HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, INFINITY, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, -INFINITY, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, INFINITY, -HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, -INFINITY, -HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, 86400.0, -HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, 86400.0, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, -86400.0, HUGE_VAL, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159, INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159, -INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159, 3.14159, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159, -INFINITY, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159, 3.14159, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159e+78, -1.175494e-53, +),
    TEST_CASE(add_d, op_d, op_d, -3.14159e+78, -1.175494e-43, +),
    TEST_CASE(add_d, op_d, op_d, 3.14159e-88, -1.175494e-40, +),
    TEST_CASE(add_d, op_d, op_d, 13.14159e+18, -13.175494e-233, +),
    TEST_CASE(add_d, op_d, op_d, -133.14159e+118, -31.175494e-3, +),
    TEST_CASE(add_d, op_d, op_d, 83.14159e-288, -1.175494e-4, +),

    TEST_CASE(greaterthan_d, op_d, op_int, 100.0, 100.0, >),
    TEST_CASE(greaterthan_d, op_d, op_int, -0.125, HUGE_VAL, >),
    TEST_CASE(greaterthan_d, op_d, op_int, HUGE_VAL, INFINITY, >),
    TEST_CASE(greaterthan_d, op_d, op_int, INFINITY, HUGE_VAL, >),
    TEST_CASE(greaterthan_d, op_d, op_int, INFINITY, NAN, >),
    TEST_CASE(greaterthan_d, op_d, op_int, NAN, INFINITY, >),
    TEST_CASE(greaterthan_d, op_d, op_int, HUGE_VAL, NAN, >),
    TEST_CASE(greaterthan_d, op_d, op_int, NAN, HUGE_VAL, >),
    TEST_CASE(greaterthan_d, op_d, op_int, HUGE_VAL, HUGE_VAL, >),
    TEST_CASE(greaterthan_d, op_d, op_int, NAN, NAN, >),
    TEST_CASE(greaterthan_d, op_d, op_int, INFINITY, INFINITY, >),
    TEST_CASE(greaterthan_d, op_d, op_int, NAN, 0.0, >),
    TEST_CASE(greaterthan_d, op_d, op_int, HUGE_VAL, 0.0, >),
    TEST_CASE(greaterthan_d, op_d, op_int, INFINITY, 0.0, >),
    TEST_CASE(greaterthan_d, op_d, op_int, 0.0, NAN, >),
    TEST_CASE(greaterthan_d, op_d, op_int, 0.0, HUGE_VAL, >),
    TEST_CASE(greaterthan_d, op_d, op_int, 0.0, INFINITY, >),
    TEST_CASE(greaterthan_d, op_d, op_int, 0.0, 0.0, >),

    TEST_CASE(greaterthanorequal_d, op_d, op_int, 100.0, 100.0, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, -0.125, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, HUGE_VAL, INFINITY, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, INFINITY, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, INFINITY, NAN, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, NAN, INFINITY, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, HUGE_VAL, NAN, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, NAN, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, HUGE_VAL, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, NAN, NAN, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, INFINITY, INFINITY, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, NAN, 0.0, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, HUGE_VAL, 0.0, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, INFINITY, 0.0, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, 0.0, NAN, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, 0.0, HUGE_VAL, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, 0.0, INFINITY, >=),
    TEST_CASE(greaterthanorequal_d, op_d, op_int, 0.0, 0.0, >=),

    TEST_CASE(lessthan_d, op_d, op_int, 100.0, 100.0, <),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, -0.125, <),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, INFINITY, <),
    TEST_CASE(lessthan_d, op_d, op_int, INFINITY, HUGE_VAL, <),
    TEST_CASE(lessthan_d, op_d, op_int, INFINITY, NAN, <),
    TEST_CASE(lessthan_d, op_d, op_int, NAN, INFINITY, <),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, NAN, <),
    TEST_CASE(lessthan_d, op_d, op_int, NAN, HUGE_VAL, <),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, HUGE_VAL, <),
    TEST_CASE(lessthan_d, op_d, op_int, NAN, NAN, <),
    TEST_CASE(lessthan_d, op_d, op_int, INFINITY, INFINITY, <),
    TEST_CASE(lessthan_d, op_d, op_int, NAN, 0.0, <),
    TEST_CASE(lessthan_d, op_d, op_int, HUGE_VAL, 0.0, <),
    TEST_CASE(lessthan_d, op_d, op_int, INFINITY, 0.0, <),
    TEST_CASE(lessthan_d, op_d, op_int, 0.0, NAN, <),
    TEST_CASE(lessthan_d, op_d, op_int, 0.0, HUGE_VAL, <),
    TEST_CASE(lessthan_d, op_d, op_int, 0.0, INFINITY, <),
    TEST_CASE(lessthan_d, op_d, op_int, 0.0, 0.0, <),


    TEST_CASE(lessthanorequal_d, op_d, op_int, 100.0, 100.0, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, HUGE_VAL, -0.125, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, HUGE_VAL, INFINITY, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, INFINITY, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, INFINITY, NAN, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, NAN, INFINITY, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, HUGE_VAL, NAN, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, NAN, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, HUGE_VAL, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, NAN, NAN, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, INFINITY, INFINITY, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, NAN, 0.0, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, HUGE_VAL, 0.0, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, INFINITY, 0.0, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, 0.0, NAN, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, 0.0, HUGE_VAL, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, 0.0, INFINITY, <=),
    TEST_CASE(lessthanorequal_d, op_d, op_int, 0.0, 0.0, <=),

    TEST_CASE_CAST(f_to_int, op_f, op_int, 0.4f),
    TEST_CASE_CAST(f_to_int, op_f, op_int, -1.1f),
    TEST_CASE_CAST(f_to_int, op_f, op_int, HUGE_VALF),
    TEST_CASE_CAST(f_to_int, op_f, op_int, -8.06f),
    TEST_CASE_CAST(f_to_int, op_f, op_int, -2.7e+2f), 
    TEST_CASE_CAST(f_to_int, op_f, op_int, -2.7e-10f), 
    TEST_CASE_CAST(f_to_int, op_f, op_int, -27272727272727.0f), 
    TEST_CASE_CAST(f_to_int, op_f, op_int, -2.7e+10f), 
    TEST_CASE_CAST(f_to_int, op_f, op_int, 2.7e+10f), 
    TEST_CASE_CAST(f_to_int, op_f, op_int, 18e+10f),
    TEST_CASE_CAST(f_to_int, op_f, op_int, 1386.0f),  
    TEST_CASE_CAST(f_to_int, op_f, op_int, -1386.0f),  
    TEST_CASE_CAST(f_to_int, op_f, op_int, INFINITY),
    TEST_CASE_CAST(f_to_int, op_f, op_int, 3.402823e+38f),
    TEST_CASE_CAST(f_to_int, op_f, op_int, -1.175494e-38f),
    TEST_CASE_CAST(f_to_int, op_f, op_int, 0.0f),

    TEST_CASE_CAST(f_to_s64, op_f, op_s64, 0.4f),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, -1.1f),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, HUGE_VALF),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, -8.06f), 
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, INFINITY),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, 3.402823e+38f),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, -1.175494e-38f),
    TEST_CASE_CAST(f_to_s64, op_f, op_s64, 0.0f),

    TEST_CASE_CAST(f_to_u64, op_f, op_u64, 0.4f),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, -1.1f),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, HUGE_VALF),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, -8.06f), 
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, INFINITY),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, 3.402823e+38f),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, -1.175494e-38f),
    TEST_CASE_CAST(f_to_u64, op_f, op_u64, 0.0f),

    TEST_CASE_CAST(f_to_s32, op_f, op_s32, 0.4f),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, -1.1f),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, HUGE_VALF),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, -8.06f), //a bunch of just random number cases
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, INFINITY),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, 3.402823e+38f),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, -1.175494e-38f),
    TEST_CASE_CAST(f_to_s32, op_f, op_s32, 0.0f),

    TEST_CASE_CAST(f_to_u32, op_f, op_u32, 0.4f),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, -1.1f),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, HUGE_VALF),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, -8.06f), //a bunch of just random number cases
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, INFINITY),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, 3.402823e+38f),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, -1.175494e-38f),
    TEST_CASE_CAST(f_to_u32, op_f, op_u32, 0.0f),

    TEST_CASE_CAST(int_to_f, op_int, op_f, 1),
    TEST_CASE_CAST(int_to_f, op_int, op_f, -1),
    TEST_CASE_CAST(int_to_f, op_int, op_f, 12),
    TEST_CASE_CAST(int_to_f, op_int, op_f, -12),
    TEST_CASE_CAST(int_to_f, op_int, op_f, 1000000),
    TEST_CASE_CAST(int_to_f, op_int, op_f, -1000000),
    TEST_CASE_CAST(int_to_f, op_int, op_f, 0xefff),
    TEST_CASE_CAST(int_to_f, op_int, op_f, 1234567),
    TEST_CASE_CAST(int_to_f, op_int, op_f, -1234567),
    TEST_CASE_CAST(int_to_f, op_int, op_f, 2034567),
    TEST_CASE_CAST(int_to_f, op_int, op_f, -2034567),

    TEST_CASE_CAST(s64_to_f, op_s64, op_f, 0x7eeeeeeeffffffff),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, 0xfeeeeeeeffffffff),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, 10000000),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, -10000000),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, 40000000),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, -40000000),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, 880000000),
    TEST_CASE_CAST(s64_to_f, op_s64, op_f, -880000000),

    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 12),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 48),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 1024),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 8838),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 0xffffffffffffffff),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 0xeffffffffffffff),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 0xfeefffffffff0000),
    TEST_CASE_CAST(u64_to_f, op_u64, op_f, 0x1234566789aabbcc),

    TEST_CASE_CAST(s32_to_f, op_s32, op_f, 12),
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, -12),
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, 156),
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, -125), //a bunch of just random number cases
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, 2000000),
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, -2000000),
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, 1001111),
    TEST_CASE_CAST(s32_to_f, op_s32, op_f, -1000022),

    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 48),
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 0),
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 1355678),
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 48567), //a bunch of just random number cases
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 1),
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 1000000),
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 18456),
    TEST_CASE_CAST(u32_to_f, op_u32, op_f, 987654),

    TEST_CASE_CAST(d_to_int, op_d, op_int, 0.4),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -1.1),
    TEST_CASE_CAST(d_to_int, op_d, op_int, HUGE_VALF),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -8.06),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -2.7e+2),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -2.7e-10),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -27272727272727.0),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -2.7e+10),
    TEST_CASE_CAST(d_to_int, op_d, op_int, 2.7e+10),
    TEST_CASE_CAST(d_to_int, op_d, op_int, 18e+10),
    TEST_CASE_CAST(d_to_int, op_d, op_int, 1386.0),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -1386.0),
    TEST_CASE_CAST(d_to_int, op_d, op_int, INFINITY),
    TEST_CASE_CAST(d_to_int, op_d, op_int, 3.402823e+38),
    TEST_CASE_CAST(d_to_int, op_d, op_int, -1.175494e-38),
    TEST_CASE_CAST(d_to_int, op_d, op_int, 0.0),

    TEST_CASE_CAST(d_to_s64, op_d, op_s64, 0.4),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, -1.1),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, HUGE_VALF),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, -8.06),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, INFINITY),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, 3.402823e+38),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, -1.175494e-38),
    TEST_CASE_CAST(d_to_s64, op_d, op_s64, 0.0),

    TEST_CASE_CAST(d_to_u64, op_d, op_u64, 0.4),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, -1.1),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, HUGE_VALF),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, -8.06),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, INFINITY),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, 3.402823e+38),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, -1.175494e-38),
    TEST_CASE_CAST(d_to_u64, op_d, op_u64, 0.0),

    TEST_CASE_CAST(d_to_s32, op_d, op_s32, 0.4),
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, -1.1),
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, HUGE_VALF),
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, -8.06), //a bunch of just random number cases
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, INFINITY),
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, 3.402823e+38),
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, -1.175494e-38),
    TEST_CASE_CAST(d_to_s32, op_d, op_s32, 0.0),

    TEST_CASE_CAST(d_to_u32, op_d, op_u32, 0.4),
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, -1.1),
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, HUGE_VALF),
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, -8.06), //a bunch of just random number cases
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, INFINITY),
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, 3.402823e+38),
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, -1.175494e-38),
    TEST_CASE_CAST(d_to_u32, op_d, op_u32, 0.0),

    TEST_CASE_CAST(int_to_d, op_int, op_d, 1),
    TEST_CASE_CAST(int_to_d, op_int, op_d, -1),
    TEST_CASE_CAST(int_to_d, op_int, op_d, 12),
    TEST_CASE_CAST(int_to_d, op_int, op_d, -12),
    TEST_CASE_CAST(int_to_d, op_int, op_d, 1000000),
    TEST_CASE_CAST(int_to_d, op_int, op_d, -1000000),
    TEST_CASE_CAST(int_to_d, op_int, op_d, 0xefff),
    TEST_CASE_CAST(int_to_d, op_int, op_d, 1234567),
    TEST_CASE_CAST(int_to_d, op_int, op_d, -1234567),
    TEST_CASE_CAST(int_to_d, op_int, op_d, 2034567),
    TEST_CASE_CAST(int_to_d, op_int, op_d, -2034567),

    TEST_CASE_CAST(s64_to_d, op_s64, op_d, 0x7eeeeeeeffffffff),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, 0xfeeeeeeeffffffff),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, 10000000),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, -10000000),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, 40000000),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, -40000000),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, 880000000),
    TEST_CASE_CAST(s64_to_d, op_s64, op_d, -880000000),

    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 12),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 48),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 1024),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 8838),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 0xffffffffffffffff),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 0xeffffffffffffff),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 0xfeefffffffff0000),
    TEST_CASE_CAST(u64_to_d, op_u64, op_d, 0x1234566789aabbcc),

    TEST_CASE_CAST(s32_to_d, op_s32, op_d, 12),
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, -12),
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, 156),
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, -125), //a bunch of just random number cases
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, 2000000),
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, -2000000),
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, 1001111),
    TEST_CASE_CAST(s32_to_d, op_s32, op_d, -1000022),

    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 48),
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 0),
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 1355678),
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 48567), //a bunch of just random number cases
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 1),
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 1000000),
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 18456),
    TEST_CASE_CAST(u32_to_d, op_u32, op_d, 987654)

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
    int i;
    int total = sizeof(tests) / sizeof(test_t);
    for(i=0; i < total; i++){
        exec_test(i);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%llu != %llu...", t.op_u64, result.op_u64);
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

    printf("%llu != %llu...", t.op_u64, result.op_u64);
    return 0;
}
/*
 *  shift to left register <<
 */
int shift_to_left_u64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_u64 = a.op_u64 << b.op_u64;

    if( t.op_u64 == result.op_u64 )
    {
        return 1;
    }

    printf("%llu != %llu...", t.op_u64, result.op_u64);
    return 0;
}
/*
 *  shift to right register >>
 */
int shift_to_right_u64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_u64 = a.op_u64 >> b.op_u64;

    if( t.op_u64 == result.op_u64 )
    {
        return 1;
    }

    printf("%llu != %llu...", t.op_u64, result.op_u64);
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

    printf("%llu != %llu...", t.op_u64, result.op_u64);
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

    printf("%llu != %llu...", t.op_u64, result.op_u64);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%llu != %llu...", t.op_u64, result.op_u64);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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


    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%lld != %lld...", t.op_s64, result.op_s64);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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


    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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


    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%d != %d...", t.op_int, result.op_int);
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

    printf("%llu != %llu...", t.op_u64, result.op_u64);
    return 0;
}


//		float to s64

int f_to_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = (s64)a.op_f;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}


//		float to s32

int f_to_s32(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s32 = (s32)a.op_f;

    if( t.op_s32 == result.op_s32 )
    {
        return 1;
    }

    printf("%ld!= %ld...", t.op_s32, result.op_s32);
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

    printf("%lu != %lu...", t.op_u32, result.op_u32);
    return 0;
}
//to float
int int_to_f(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_f = (float)a.op_int;
    if( t.op_f == result.op_f )
    {
        return 1;
    }
    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}

int u64_to_f(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_f = (float)a.op_u64;
    if( t.op_f == result.op_f )
    {
        return 1;
    }
    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}

int s64_to_f(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_f = (float)a.op_s64;
    if( t.op_f == result.op_f )
    {
        return 1;
    }
    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}

int s32_to_f(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_f = (float)a.op_s32;
    if( t.op_f == result.op_f )
    {
        return 1;
    }
    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}

int u32_to_f(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_f = (float)a.op_u32;
    if( t.op_f == result.op_f )
    {
        return 1;
    }
    printf("%f != %f...", t.op_f, result.op_f);
    return 0;
}
//from double
int d_to_int(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_int = (int)a.op_d;
    if( t.op_int == result.op_int )
    {
        return 1;
    }
    printf("%d != %d...", t.op_int, result.op_int);
    return 0;
}

int d_to_u64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_u64 = (u64)a.op_d;

    if( t.op_u64 == result.op_u64 )
    {
        return 1;
    }

    printf("%llu != %llu...", t.op_u64, result.op_u64);
    return 0;
}

int d_to_s64(operand_t a, operand_t b, operand_t result)
{
    operand_t t;

    t.op_s64 = (s64)a.op_d;

    if( t.op_s64 == result.op_s64 )
    {
        return 1;
    }

    printf("%lld != %lld...", t.op_s64, result.op_s64);
    return 0;
}

int d_to_s32(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_s32 = (s32)a.op_d;
    if( t.op_s32 == result.op_s32 )
    {
        return 1;
    }
    printf("%ld!= %ld...", t.op_s32, result.op_s32);
    return 0;
}

int d_to_u32(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_u32 = (u32)a.op_d;
    if( t.op_u32 == result.op_u32 )
    {
        return 1;
    }
    printf("%lu != %lu...", t.op_u32, result.op_u32);
    return 0;
}
//to double
int int_to_d(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_d = (double)a.op_int;
    if( t.op_d == result.op_d )
    {
        return 1;
    }
    printf("%lf != %lf...", t.op_d, result.op_d);
    return 0;
}

int u64_to_d(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_d = (double)a.op_u64;
    if( t.op_d == result.op_d )
    {
        return 1;
    }
    printf("%lf != %lf...", t.op_d, result.op_d);
    return 0;
}

int s64_to_d(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_d = (double)a.op_s64;
    if( t.op_d == result.op_d )
    {
        return 1;
    }
    printf("%lf != %lf...", t.op_d, result.op_d);
    return 0;
}

int s32_to_d(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_d = (double)a.op_s32;
    if( t.op_d == result.op_d )
    {
        return 1;
    }
    printf("%lf != %lf...", t.op_d, result.op_d);
    return 0;
}

int u32_to_d(operand_t a, operand_t b, operand_t result)
{
    operand_t t;
    t.op_d = (double)a.op_u32;
    if( t.op_d == result.op_d )
    {
        return 1;
    }
    printf("%lf != %lf...", t.op_d, result.op_d);
    return 0;
}
