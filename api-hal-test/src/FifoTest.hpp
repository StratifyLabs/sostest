#ifndef FIFO_TEST_HPP
#define FIFO_TEST_HPP
#include <sapi/hal.hpp>
#include <sapi/test.hpp>

class FifoTest : public Test {
public:
    FifoTest();

    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();


};

#endif // FIFO_TEST_HPP
