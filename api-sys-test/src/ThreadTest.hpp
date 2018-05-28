#ifndef THREADTEST_HPP
#define THREADTEST_HPP

#include <sapi/test.hpp>

class ThreadTest : public Test {
public:
    ThreadTest();

private:
    //api
    static int count_0;
    static int count_1;
    static int count_2;
    const u32 wait_time = 4000;

    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();
    static void * handle_thread_0(void * args){
        ThreadTest * object = (ThreadTest*)args;
        return object->thread_0(object->wait_time);
    }
    static void * handle_thread_1(void * args){
        ThreadTest * object = (ThreadTest*)args;
        return object->thread_1(object->wait_time);
    }
    static void * handle_thread_2(void * args){
        ThreadTest * object = (ThreadTest*)args;
        return object->thread_2(object->wait_time);
    }

    void * thread_0(u32);
    void * thread_1(u32);
    void * thread_2(u32);
};

#endif // THREADTEST_HPP
