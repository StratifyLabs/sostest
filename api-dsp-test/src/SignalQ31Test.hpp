#ifndef SIGNALQ31TEST_HPP
#define SIGNALQ31TEST_HPP

#include <sapi/test.hpp>

class SignalQ31Test : public Test {
public:
    SignalQ31Test();

    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // SIGNALQ31TEST_HPP
