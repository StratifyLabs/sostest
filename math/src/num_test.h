#ifndef NUM_TEST_H
#define NUM_TEST_H

#include <mcu/types.h>

enum {
    NUM_TEST_FLAG_INT = (1<<0),
    NUM_TEST_FLAG_FLOAT = (1<<1),
    NUM_TEST_FLAG_ALL = 0xffffffff
};

#if defined __cplusplus
extern "C" {
#endif

int num_test_execute(u32 o_flags);

#if defined __cplusplus
}
#endif

#endif // NUM_TEST_H
