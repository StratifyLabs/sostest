#ifndef TASKTEST_HPP
#define TASKTEST_HPP

#include <sapi/test.hpp>

class TaskTest:public Test {
public:
    TaskTest();
private:
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();

};

#endif // TASKTEST_HPP
