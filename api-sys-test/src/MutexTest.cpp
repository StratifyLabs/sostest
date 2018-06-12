#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>
#include "MutexTest.hpp"
static u32 count_0 = 0;
static u32 count_1 = 0;
static u32 count_2 = 0;
static u32 count_common;
static u32 wait_time_1 = 800;
static u32 wait_time_2 = 800;
static void * thread_1(void * args);
static void * thread_2(void * args);
static int common_use_function(int id);
static int uno_id,dos_id,common_id;
static Mutex mutex_test;

MutexTest::MutexTest():Test("sys::Mutex")
{

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
    Thread uno_thread,dos_thread;
    enum Sched::policy uno_policy,dos_policy;
    uno_policy = Sched::RR;dos_policy = Sched::FIFO;
    int uno_priority,dos_priority;
    uno_priority =1, dos_priority = 1;
    MutexAttr mutex_attr;
    if(!execute_api_mutex_attr_case(&mutex_attr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mutex_test.set_attr(mutex_attr);
    if(mutex_test.lock()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((uno_thread.create(thread_1,this,uno_priority,uno_policy)==-1)||
        (dos_thread.create(thread_2,this,dos_priority,dos_policy)==-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    Timer::wait_microseconds(100);
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
    if(uno_thread.is_running() && uno_thread.is_valid()){
        uno_thread.kill(0);
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.is_valid() && dos_thread.is_running()){
        dos_thread.kill(0);
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;
}
/*  called in MutexTest::execute_class_api_case(){
 * */
bool MutexTest::execute_api_mutex_attr_case(MutexAttr * mutex_attr){
    bool result = true;
    int prio_ceiling_test;
    bool pshared_test;
    MutexAttr::type attr_type;
    MutexAttr::protocol attr_protocol;
    pshared_test = false;
    prio_ceiling_test=0;
    attr_type = MutexAttr::RECURSIVE;
    MutexAttr mutex_attr_test(attr_type,prio_ceiling_test,pshared_test);
    if((mutex_attr_test.get_type()!= attr_type)||
       (mutex_attr_test.get_prio_ceiling() !=prio_ceiling_test)||
        (mutex_attr_test.get_pshared()!= pshared_test)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_type = MutexAttr::NORMAL;
    mutex_attr_test.set_type(attr_type);
    pshared_test = true;
    prio_ceiling_test = 0;
    mutex_attr_test.set_pshared(pshared_test);
    mutex_attr_test.set_prio_ceiling(prio_ceiling_test);
    if((mutex_attr_test.get_type()!= attr_type)||
       (mutex_attr_test.get_prio_ceiling() !=prio_ceiling_test)||
        (mutex_attr_test.get_pshared()!= pshared_test)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_protocol = MutexAttr::PRIO_NONE_PROTOCOL;
    mutex_attr_test.set_protocol(attr_protocol);
    if(mutex_attr_test.get_protocol()!=attr_protocol){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_protocol = MutexAttr::PRIO_PROTECT_PROTOCOL;
    mutex_attr_test.set_protocol(attr_protocol);
    if(mutex_attr_test.get_protocol()!=attr_protocol){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_protocol = MutexAttr::PRIO_INHERIT_PROTOCOL;
    mutex_attr_test.set_protocol(attr_protocol);
    if(mutex_attr_test.get_protocol()!=attr_protocol){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mutex_attr->set_protocol((MutexAttr::protocol)mutex_attr_test.get_protocol());//PRIO_INHERIT_PROTOCOL
    mutex_attr->set_prio_ceiling(mutex_attr_test.get_prio_ceiling());//0
    mutex_attr->set_pshared(mutex_attr_test.get_pshared());//true
    mutex_attr->set_type((MutexAttr::type)mutex_attr_test.get_type());//NORMAL
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
    const u32 itteration =1000;
    Thread uno_thread,dos_thread;
    enum Sched::policy uno_policy,dos_policy;
    uno_policy = Sched::FIFO;dos_policy = Sched::FIFO;
    int uno_priority,dos_priority;
    uno_priority =1, dos_priority = 2;
    MutexAttr mutex_attr;
    if(!execute_api_mutex_attr_case(&mutex_attr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mutex_attr.set_type(MutexAttr::RECURSIVE);
    mutex_attr.set_protocol(MutexAttr::PRIO_INHERIT_PROTOCOL);
    mutex_attr.set_pshared(true);
    mutex_test.set_attr(mutex_attr);
    if(mutex_test.lock()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    count_0 = 1;
    if((uno_thread.create(thread_1,this,uno_priority,uno_policy)==-1)||
        (dos_thread.create(thread_2,this,dos_priority,dos_policy)==-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    uno_id = uno_thread.id();
    dos_id = dos_thread.id();

    if(mutex_test.unlock()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    /*while((count_1==0) || (count_2==0)){
        Thread::yield();
        Timer::wait_microseconds(100);
    }*/
    count_1 =1;
    count_2 =1;
    for(u32 i=0;i<itteration;i++){
        if(mutex_test.lock()<0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }else{
            //print_case_message("itterate %d:%d:%d:%d",count_common,i,count_1,count_2);
        }
        Timer::wait_microseconds(500);
        count_0++;
        if(mutex_test.unlock()<0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        Thread::yield();
        Timer::wait_microseconds(5000);
    }
    if(count_1==1 || count_2==1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        print_case_message("count %d:%d:%d",count_0,count_1,count_2);
        result = false;
    }


    if(uno_thread.is_running()){
        uno_thread.kill(0);
    }else{

        result = false;
    }
    if(dos_thread.is_running()){
        dos_thread.kill(0);
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

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
    return result;
}

static void * thread_1(void * args){
    mutex_test.lock();
    if(count_0==0){
        count_1 = 0;
        mutex_test.unlock();
        return &count_1;
    }
    count_1++;
    Timer::wait_microseconds(wait_time_1);
    mutex_test.unlock();
    count_1++;
    while(1){
        if (common_use_function(uno_id)){
            count_1=0;
            break;
        }else{
            count_1++;
        }
        Timer::wait_microseconds(wait_time_1);
    }
    return &count_1;
}

static void * thread_2(void * args){
    mutex_test.lock();
    if(count_0==0){
        count_2 = 0;
        mutex_test.unlock();
        return &count_2;
    }
    count_2++;
    Timer::wait_microseconds(wait_time_2);
    mutex_test.unlock();
    count_2++;
    while(1){
        if (common_use_function(dos_id)){
            count_2=0;
            break;
        }else{
            count_2++;
        }
        Timer::wait_microseconds(wait_time_2);
    }
    return &count_2;
}
static int common_use_function(int id){
    mutex_test.lock();
    common_id = id;
    Timer::wait_microseconds(wait_time_2);
    u8 itterate = 100;
    for (itterate=itterate;itterate;itterate--){
        Timer::wait_microseconds(wait_time_2);
        if((int)Thread::self()!=common_id){
            break;
        }
    }
    count_common++;
    mutex_test.unlock();
    return itterate;
}
