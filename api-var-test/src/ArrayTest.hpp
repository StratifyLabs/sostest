#ifndef ARRAYTEST_HPP
#define ARRAYTEST_HPP

#include <sapi/test.hpp>

class ArrayTest : public Test {
public:
    ArrayTest();

private:
    //cases
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
    bool execute_recursive(Array <u64,64>array);
    static int recursive_number;
};

#endif // TOKENTEST_HPP
