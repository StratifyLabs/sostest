#ifndef SIGNALTEST_HPP
#define SIGNALTEST_HPP

#include <sapi/sys.hpp>
#include <sapi/test.hpp>


class SignalTest : public Test {
public:
    u32 wait_time = 4000;
    SignalTest();
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();
private:
    static void * handle_thread_1(void * args){
        SignalTest * object = (SignalTest*)args;
        return object->thread_1(object->wait_time);
    }
    void * thread_1(u32);
};

#endif // SIGNALTEST_HPP
