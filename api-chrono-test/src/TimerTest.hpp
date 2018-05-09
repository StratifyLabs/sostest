#ifndef MICROTIMERTEST_HPP
#define MICROTIMERTEST_HPP

#include <sapi/test.hpp>


class TimerTest : public Test
{
public:
    TimerTest();

    bool execute_class_api_case();
    bool execute_class_stress_case();
};

#endif // MICROTIMERTEST_HPP
