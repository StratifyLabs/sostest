#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>
#include <sapi/var.hpp>
#include "MutexTest.hpp"
#include "ThreadTest.hpp"
static volatile u32 count_0 = 0;
static volatile u32 count_1 = 0;
static volatile u32 count_2 = 0;
static u32 wait_time_0 = 500;
static u32 wait_time_1 = 1000;
static u32 wait_time_2 = 3000;
static void * thread_1(void * args);
static void * thread_2(void * args);
static int uno_id,dos_id;
static Mutex mutex_test;
static volatile bool stop_threads = false;

MutexTest::MutexTest():Test("sys::Mutex"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/Mutex use "api-sys-test -mutex -api"
 * Mutex
 *  set_attr,lock,unlock
 * MutexAttr
 *  get_type,get_prio_ceiling,get_pshared,get_protocol
 *  set_type,set_prio_ceiling,set_pshared,set_protocol
 *
 * not writed test
 *  lock_timed
 * * @todo more performance test for size prio and flags
 * not tested on
 * @style in mutex_attr get_type() use "int" but set_type() use enum type
 * @style in mutex_attr get_protocol() use "int" but set_protocok use enum protocol
 */
bool MutexTest::execute_class_api_case(){
    bool result = true;
    Thread uno_thread(Thread::StackSize(2048)),
          dos_thread(Thread::StackSize(2048));
    enum Sched::policy uno_policy,dos_policy;
    uno_policy = Sched::RR;dos_policy = Sched::FIFO;
    int uno_priority,dos_priority;
    uno_priority =1; dos_priority = 1;
    MutexAttr mutex_attr;
    if(!execute_api_mutex_attr_case(&mutex_attr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mutex_test.set_attributes(mutex_attr);
    if(mutex_test.lock()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    stop_threads = false;

    TEST_THIS_EXPECT(bool,
                    uno_thread.create(
                       Thread::Function(thread_1),
                       Thread::FunctionArgument(this),
                       Thread::Priority(uno_priority),
                       uno_policy
                       ) >= 0,
                    true);

    TEST_THIS_EXPECT(bool,
                    dos_thread.create(
                       Thread::Function(thread_2),
                       Thread::FunctionArgument(this),
                       Thread::Priority(dos_priority),
                       dos_policy
                       ) >= 0,
                    true);

    wait(Microseconds(100));
    count_0++;
    if(mutex_test.unlock()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    /*ClockTime clock_time;
    u32 nano_wait = 1000000;
    clock_time.set(0,nano_wait);
    if(mutex_test.lock_timed(clock_time)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }*/

    stop_threads = true;

    print_case_message("Wait for uno thread");
    uno_thread.wait();
    print_case_message("Wait for dos thread");
    dos_thread.wait();

    return result;
}
/*  called in MutexTest::execute_class_api_case(){
 * */
bool MutexTest::execute_api_mutex_attr_case(MutexAttr * mutex_attr){
    bool result = true;
    int prio_ceiling_test;
    bool pshared_test;
    MutexAttributes::type attr_type;
    MutexAttributes::protocol attr_protocol;
    pshared_test = false;
    prio_ceiling_test=0;
    attr_type = MutexAttributes::RECURSIVE;

    MutexAttributes mutex_attr_test;
    mutex_attr_test.set_type(attr_type)
          .set_prio_ceiling(prio_ceiling_test)
          .set_pshared(pshared_test);

    TEST_THIS_EXPECT(int, mutex_attr_test.get_type(), attr_type);
    TEST_THIS_EXPECT(int, mutex_attr_test.get_prio_ceiling(), prio_ceiling_test);
    TEST_THIS_EXPECT(int, mutex_attr_test.get_pshared(), pshared_test);

    attr_type = MutexAttr::NORMAL;
    mutex_attr_test.set_type(attr_type);

    pshared_test = true;
    mutex_attr_test.set_pshared(pshared_test);

    prio_ceiling_test = 0;
    mutex_attr_test.set_prio_ceiling(prio_ceiling_test);

    TEST_THIS_EXPECT(int, mutex_attr_test.get_type(), attr_type);
    TEST_THIS_EXPECT(int, mutex_attr_test.get_prio_ceiling(), prio_ceiling_test);
    TEST_THIS_EXPECT(int, mutex_attr_test.get_pshared(), pshared_test);

    attr_protocol = MutexAttr::PRIO_NONE_PROTOCOL;
    mutex_attr_test.set_protocol(attr_protocol);
    TEST_THIS_EXPECT(int, mutex_attr_test.get_protocol(), attr_protocol);
    attr_protocol = MutexAttr::PRIO_PROTECT_PROTOCOL;
    TEST_THIS_EXPECT(int, mutex_attr_test.get_protocol(), attr_protocol);

    attr_protocol = MutexAttr::PRIO_INHERIT_PROTOCOL;
    mutex_attr_test.set_protocol(attr_protocol);
    TEST_THIS_EXPECT(int, mutex_attr_test.get_protocol(), attr_protocol);

    mutex_attr->set_protocol((MutexAttr::protocol)mutex_attr_test.get_protocol());
    mutex_attr->set_prio_ceiling(mutex_attr_test.get_prio_ceiling());
    mutex_attr->set_pshared(mutex_attr_test.get_pshared());
    mutex_attr->set_type((MutexAttr::type)mutex_attr_test.get_type());

    return result;
}

/*@brief stress test for sys/Mutex use "api-sys-test -mutex -stress"
 *
 *
 * not writed test
 *  lock_timed
 * not tested on
 * thread1 and thread2 not lock mutex
 */
bool MutexTest::execute_class_stress_case(){
    bool result = true;
    const u32 itteration = 200;
    Thread uno_thread(Thread::StackSize(2048)),
          dos_thread(Thread::StackSize(2048));

    enum Sched::policy uno_policy,dos_policy;
    uno_policy = Sched::FIFO;dos_policy = Sched::FIFO;
    int uno_priority,dos_priority;
    uno_priority = 3; dos_priority = 3;
    MutexAttr mutex_attr;
    if(!execute_api_mutex_attr_case(&mutex_attr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    mutex_attr.set_type(MutexAttr::RECURSIVE);
    mutex_attr.set_protocol(MutexAttr::PRIO_INHERIT_PROTOCOL);
    mutex_attr.set_pshared(false);
    mutex_attr.set_prio_ceiling(dos_priority > uno_priority ? dos_priority : uno_priority);
    mutex_test.set_attributes(mutex_attr);

    TEST_THIS_EXPECT(bool, mutex_test.lock()<0, false);

    count_0 = 1;
    stop_threads = false;

    TEST_THIS_EXPECT(bool,
                    uno_thread.create(
                       Thread::Function(thread_1),
                       Thread::FunctionArgument(this),
                       Thread::Priority(uno_priority),
                       uno_policy
                       ) >= 0,
                    true);
    TEST_THIS_EXPECT(bool,
                    dos_thread.create(
                       Thread::Function(thread_2),
                       Thread::FunctionArgument(this),
                       Thread::Priority(dos_priority),
                       dos_policy
                       ) >= 0,
                    true);

    TEST_THIS_EXPECT(bool, mutex_test.unlock()<0, false);


    for(u8 j=0;j<7;j++){
        uno_policy = test_get_policy(j%3);dos_policy = test_get_policy((j|0x01)%3);
        uno_priority = rand()%10; dos_priority = rand()%10;

        TEST_THIS_EXPECT(bool, uno_thread.set_priority(Thread::Priority(uno_priority), uno_policy)<0, false);
        TEST_THIS_EXPECT(bool, dos_thread.set_priority(Thread::Priority(dos_priority), dos_policy)<0, false);

        count_1 =1;
        count_2 =1;
        for(u32 i=0;i<itteration;i++){

           TEST_THIS_EXPECT(bool, mutex_test.lock()<0, false);

            wait(Microseconds(wait_time_0));
            count_0++;
            TEST_THIS_EXPECT(bool, mutex_test.unlock()<0, false);

            Thread::yield();
            wait(Microseconds(wait_time_0));
        }
        if(count_1<4 || count_2<4){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            print_case_message("count %d:%d:%d",count_0,count_1,count_2);
            result = false;
        }
    }
    stop_threads = true;
    uno_thread.wait();
    dos_thread.wait();
    return result;
}
/*@brief performance test for sys/Mutex use "api-sys-test -mutex -performance"
 *
 *
 * not writed test
 *  lock_timed
 * not tested on
 */
bool MutexTest::execute_class_performance_case(){
    bool result = true;
    const u32 itteration = 200;
    Thread uno_thread(Thread::StackSize(2048)),
          dos_thread(Thread::StackSize(2048));
    enum Sched::policy uno_policy,dos_policy;
    uno_policy = Sched::FIFO;dos_policy = Sched::FIFO;
    int uno_priority,dos_priority;
    uno_priority = 3; dos_priority = 3;
    MutexAttr mutex_attr;
    if(!execute_api_mutex_attr_case(&mutex_attr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mutex_attr.set_type(MutexAttr::RECURSIVE);
    mutex_attr.set_protocol(MutexAttr::PRIO_INHERIT_PROTOCOL);
    mutex_attr.set_pshared(false);
    mutex_attr.set_prio_ceiling(dos_priority > uno_priority ? dos_priority : uno_priority);
    mutex_test.set_attributes(mutex_attr);

    TEST_THIS_EXPECT(bool, mutex_test.lock()<0, false);

    if(mutex_test.lock()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    count_0 = 1;
    stop_threads = false;

    TEST_THIS_EXPECT(bool,
                    uno_thread.create(
                       Thread::Function(thread_1),
                       Thread::FunctionArgument(this),
                       Thread::Priority(uno_priority),
                       uno_policy
                       ) >= 0,
                    true);
    TEST_THIS_EXPECT(bool,
                    dos_thread.create(
                       Thread::Function(thread_2),
                       Thread::FunctionArgument(this),
                       Thread::Priority(dos_priority),
                       dos_policy
                       ) >= 0,
                    true);

    uno_id = uno_thread.id();
    dos_id = dos_thread.id();

    TEST_THIS_EXPECT(bool, mutex_test.unlock()<0, false);

    count_1 =1;
    count_2 =1;
    for(u32 i=0;i<itteration;i++){

       TEST_THIS_EXPECT(bool, mutex_test.lock()<0, false);

        if(mutex_test.lock()<0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }else{
            //print_case_message("itterate %d:%d:%d:%d",count_common,i,count_1,count_2);
        }
        wait(Microseconds(wait_time_0));
        count_0++;

        TEST_THIS_EXPECT(bool, mutex_test.unlock()<0, false);

        Thread::yield();
        wait(Microseconds(wait_time_0));
    }
    TEST_THIS_EXPECT(bool, count_1<4, false);
    TEST_THIS_EXPECT(bool, count_2<4, false);

    stop_threads = true;
    uno_thread.wait();
    dos_thread.wait();
    return result;
}

static void * thread_1(void * args){
    mutex_test.lock();
    (void)args;
    if(count_0==0){
        count_1 = 0;
        mutex_test.unlock();
        return (void*)&count_1;
    }
    count_1++;
    wait(Microseconds(wait_time_1));
    mutex_test.unlock();
    count_1++;
    while( !stop_threads ){
        mutex_test.lock();
        wait(Microseconds(wait_time_1));
        count_1++;
        mutex_test.unlock();
        wait(Microseconds(wait_time_1));
    }
    return (void*)&count_1;
}

static void * thread_2(void * args){
    mutex_test.lock();
    if(count_0==0){
        count_2 = 0;
        mutex_test.unlock();
        return (void*)&count_2;
    }
    count_2++;
    wait(Microseconds(wait_time_2));
    mutex_test.unlock();
    count_2++;
    while( !stop_threads ){
        mutex_test.lock();
        wait(Microseconds(wait_time_2));
        count_2++;
        mutex_test.unlock();
        wait(Microseconds(wait_time_2));
    }
    return (void*)&count_2;
}

