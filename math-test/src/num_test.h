#ifndef NUM_TEST_H
#define NUM_TEST_H

#include <mcu/types.h>
enum {
    U8_TEST_FLAG = (1<<5),
    U16_TEST_FLAG = (1<<6),
    U32_TEST_FLAG = (1<<7),
    U64_TEST_FLAG = (1<<8),
    S8_TEST_FLAG = (1<<9),
    S16_TEST_FLAG = (1<<10),
    S32_TEST_FLAG = (1<<11),
    S64_TEST_FLAG = (1<<12),
    FLOAT_TEST_FLAG = (1<<13),
    DOUBLE_TEST_FLAG = (1<<14),
    INT_TEST_FLAG = (1<<15)
};

enum {
    EXECUTE_ALL_TEST_FLAG = 0x1f,
    COMPARISON_TEST_FLAG = (1<<0),
    ARITHMETIC_TEST_FLAG = (1<<1),
    LOGICAL_TEST_FLAG = (1<<2),
    SHIFT_TEST_FLAG = (1<<3),
    TYPE_CAST_TEST_FLAG = (1<<4)
};

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
    const char * a_str_type;
    const char * b_str;
    const char * op_str;
    op operation;
    operand_t a;
    operand_t b;
    operand_t result;
    u32 o_execute_flags;
} test_t;

#if defined __cplusplus
extern "C" {
#endif

int num_test_execute(u32 o_flags,u32 o_execute_flags);

const test_t * num_test_get(u32 idx);
u32 num_test_count();

#if defined __cplusplus
}
#endif

#endif // NUM_TEST_H
