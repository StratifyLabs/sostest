#ifndef CLITEST_HPP
#define CLITEST_HPP

#include <sapi/test.hpp>

class CliTest:public Test {
public:
    u32 wait_time = 4000;/*!<wait time for object thread*/
    CliTest();
private:
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();
private:
    static void * handle_thread_1(void * args){
        CliTest * object = (CliTest*)args;
        return object->thread_1(object->wait_time);
    }
    void * thread_1(u32);

};

#endif // TASKTEST_HPP
