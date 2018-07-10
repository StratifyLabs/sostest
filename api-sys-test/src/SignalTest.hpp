#ifndef SIGNALTEST_HPP
#define SIGNALTEST_HPP
#include <sapi/sys.hpp>
#include <sapi/test.hpp>

class SignalTest : public Test {
public:
    SignalTest();
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();
private:

};

#endif // SIGNALTEST_HPP
