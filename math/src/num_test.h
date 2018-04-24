#ifndef NUM_TEST_H
#define NUM_TEST_H

#include <mcu/types.h>
enum {
    U8_TEST_FLAG = (1<<5),
    U16_TEST_FLAG = (1<<6),
    U32_TEST_FLAG = (1<<7),
    U64_TEST_FLAG = (1<<8),
    I8_TEST_FLAG = (1<<9),
    S16_TEST_FLAG = (1<<10),
    S32_TEST_FLAG = (1<<11),
    S64_TEST_FLAG = (1<<12),
    FLOAT_TEST_FLAG = (1<<13),
    DOUBLE_TEST_FLAG = (1<<14),
    INT_TEST_FLAG = (1<<15)
};

enum {
    EXECUTE_ALL_TEST_FLAG = 0xffffffff,
    COMPARISON_TEST_FLAG = (1<<0),
    ARITHMETIC_TEST_FLAG = (1<<1),
    LOGICAL_TEST_FLAG = (1<<2),
    SHIFT_TEST_FLAG = (1<<3),
    TYPE_CAST_TEST_FLAG = (1<<4)
};


#if defined __cplusplus
extern "C" {
#endif

int num_test_execute(u32 o_flags,u32 o_execute_flags);

#if defined __cplusplus
}
#endif

#endif // NUM_TEST_H
