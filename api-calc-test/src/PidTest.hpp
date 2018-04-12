#ifndef PIDTEST_HPP
#define PIDTEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class PidTest : public Test {
public:
    PidTest();


private:
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // PIDTEST_HPP
