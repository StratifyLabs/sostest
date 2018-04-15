#ifndef RINGTEST_HPP
#define RINGTEST_HPP

#include <sapi/test.hpp>

class RingTest : public Test {
public:
    RingTest();

private:
    //cases
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

};

#endif // TOKENTEST_HPP
