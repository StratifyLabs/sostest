#ifndef PTHREADTEST_HPP
#define PTHREADTEST_HPP

#include <sapi/test.hpp>

class PThreadTest : public Test {
public:
    PThreadTest();

    bool execute_class_api_case();

private:
    static int get_available_task_count();

    static void * handle_thread0(void * args){
        PThreadTest * object = (PThreadTest*)args;
        return object->thread0();
    }

    static void * handle_thread1(void * args){
        PThreadTest * object = (PThreadTest*)args;
        return object->thread1();
    }

    void * thread0();
    void * thread1();

    int m_count;
};

#endif // PTHREADTEST_HPP
