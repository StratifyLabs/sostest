#ifndef MESSENGER_TEST_HPP
#define MESSENGER_TEST_HPP

#include <sapi/test.hpp>

class MessengerTest:public Test {
public:
    u32 wait_time = 4000;/*!<wait time for object thread*/
    MessengerTest();
private:
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();
private:
    static void * handle_thread_1(void * args){
        MessengerTest * object = (MessengerTest*)args;
        return object->thread_1(object->wait_time);
    }
    void * thread_1(u32);

};

#endif // MESSENGER_TEST_HPP
