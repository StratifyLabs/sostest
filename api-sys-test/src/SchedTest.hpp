#ifndef SCHEDTEST_HPP
#define SCHEDTEST_HPP

#include <sapi/test.hpp>

class SchedTest : public Test {
public:
    SchedTest();
private:

    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
};

#endif // SCHEDTEST_HPP
