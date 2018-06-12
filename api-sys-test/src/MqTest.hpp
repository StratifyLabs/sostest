#ifndef MQTEST_HPP
#define MQTEST_HPP

#include <sapi/test.hpp>

class MqTest : public Test {
public:
    MqTest();
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();
private:
    //mq attr
    bool execute_api_mq_attr_case(MqAttr * mq_attr);
    bool execute_api_simply_case();
    bool execute_api_thread_case();

};

#endif // MQTEST_HPP
