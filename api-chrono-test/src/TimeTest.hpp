#ifndef TIMETEST_HPP
#define TIMETEST_HPP

#include <sapi/test.hpp>

class TimeTest : public Test
{
public:
    TimeTest();

    bool execute_class_api_case();
    bool execute_class_stress_case();
};

#endif // TIMETEST_HPP
