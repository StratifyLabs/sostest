#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include "TaskTest.hpp"
#include "SchedTest.hpp"
#include "ThreadTest.hpp"

TaskTest::TaskTest() : Test("sys::Task"){
	Data::reclaim_heap_space();
}
static volatile bool stop_threads = false;
static bool thread_result = true;

/*@brief api test for sys/Signal use "api-sys-test -task -api"
 * TaskInfo
 * Task
 * not writed test
 * TaskInfo
 * TaskInfo,TaskInfo,invalid,is_valid,pid,id,thread_id,timer,is_active,priority,priority_ceiling,
 * is_thread,is_enabled,name,memory_size,heap_size,stack_size,print_header,print
 * Task
 *  Task count_total(),count_free,set_id,id,get_next,get_info,get_attr,print
 * @warning task_info_test.priority() have zero value
 */
bool TaskTest::execute_class_api_case(){
    bool result = true;
    TaskInfo  task_info_test;
	 TaskManager task_test;
    Thread thread_test;
    u32 id_test;
    Sched::policy policy = Sched::RR;
    int prio_test = Sched::get_priority_min(policy);

    SET_SCHEDULER(Sched::get_pid(), policy, prio_test);

    task_test.set_id((int)Thread::self());
    TEST_THIS_EXPECT(int, task_test.id(), Thread::self());
    task_info_test = task_test.get_info(task_test.id());
    TEST_THIS_EXPECT(int, task_test.id(), Thread::self());

    id_test = task_info_test.id();
    TEST_THIS_EXPECT(bool, task_info_test.is_active(), true);


    task_info_test = task_test.get_info(task_test.id());
    if(task_info_test.priority()!=prio_test){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        print_case_message("prio %d != %d",task_info_test.priority(),prio_test);
        result = false;
    }
    if(task_info_test.priority_ceiling()<task_info_test.priority()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(task_info_test.is_thread()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!task_info_test.is_enabled()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!task_info_test.is_enabled()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
	 if( task_info_test.name() != "api-sys-test" ){
		  print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!task_info_test.memory_size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!task_info_test.heap_size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!task_info_test.stack_size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((task_test.count_total()<task_test.count_free()) || (!task_test.count_total())){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    Thread uno_thread(Thread::StackSize(4096));
    int signal_thread_priority = 1;
    enum Sched::policy signal_thread_policy = Sched::RR;


    CREATE_THREAD(uno_thread, handle_thread_1, this, signal_thread_priority, signal_thread_policy);

    task_test.get_next(task_info_test);
    task_info_test = task_test.get_info(task_test.id());
    if(id_test == task_info_test.id()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        print_case_message("id_test %d == %d",id_test,task_info_test.id());
        result = false;
    }
    return result;
}

/*@brief stress test for sys/Signal use "api-sys-test -task -stress"
 */
bool TaskTest::execute_class_stress_case(){
    bool result = true;
    Thread thread_test;
    u32 id_test;
    u16 itterate = 100;
    Thread uno_thread(Thread::StackSize(4096));
    enum Sched::policy signal_thread_policy = Sched::RR;
    int signal_thread_priority = Sched::get_priority_min(signal_thread_policy);

    CREATE_THREAD(uno_thread, handle_thread_1, this, signal_thread_priority, signal_thread_policy);

    for(u16 i=0;i<itterate;i++) {
        TaskInfo  task_info_test;
		  TaskManager task_test;
        int priority_diff;
        priority_diff = Sched::get_priority_max(signal_thread_policy) - Sched::get_priority_min(signal_thread_policy);
        priority_diff = rand()%priority_diff;
        //int signal_thread_priority = Sched::get_priority_min(signal_thread_policy) + priority_diff;
        int signal_thread_priority = Sched::get_priority_min(signal_thread_policy);

        SET_SCHEDULER(uno_thread.get_pid(),signal_thread_policy,signal_thread_priority);

        task_test.set_id((int)uno_thread.self());
        if(task_test.id() != (int)uno_thread.self()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        task_info_test = task_test.get_info(task_test.id());
		  if(task_info_test.id() != uno_thread.self()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        id_test = task_info_test.id();
        if(!task_info_test.is_active()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(task_info_test.priority()!=signal_thread_priority){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            print_case_message("prio %d:%d", task_info_test.priority(), signal_thread_priority);
            result = false;
            break;
        }
        if(task_info_test.priority_ceiling()<task_info_test.priority()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(task_info_test.is_thread()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(!task_info_test.is_enabled()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(!task_info_test.is_enabled()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
		  if( task_info_test.name() != "api-sys-test"){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(!task_info_test.memory_size()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(!task_info_test.heap_size()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if(!task_info_test.stack_size()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if((task_test.count_total()<task_test.count_free()) || (!task_test.count_total())){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        task_test.get_next(task_info_test);
        task_info_test = task_test.get_info(task_test.id());
        if(id_test == task_info_test.id()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
    }
    return result;
}

/*@brief performance test for sys/Signal use "api-sys-test -task -performance"
 * @warning delete info priority check
 */
bool TaskTest::execute_class_performance_case(){
    bool result = true;
    Thread thread_test;
    u32 id_test;
    u16 itterate = 1000;
    Thread uno_thread(Thread::StackSize(4096));
    enum Sched::policy signal_thread_policy = Sched::RR;
    int signal_thread_priority = Sched::get_priority_min(signal_thread_policy);

    CREATE_THREAD(uno_thread, handle_thread_1, this, signal_thread_priority, signal_thread_policy);

    for(u16 i=0;i<itterate;i++) {
        TaskInfo  task_info_test;
		  TaskManager task_test;
        task_test.set_id((int)uno_thread.self());

        TEST_THIS_EXPECT(int, task_test.id(), uno_thread.self());

        task_info_test = task_test.get_info(task_test.id());
        TEST_THIS_EXPECT(int, task_info_test.id(), uno_thread.self());

        id_test = task_info_test.pid();
        TEST_THIS_EXPECT(bool, task_info_test.is_active(), true);
        TEST_THIS_EXPECT(bool, task_info_test.priority_ceiling()<task_info_test.priority(), false);

        TEST_THIS_EXPECT(bool, task_info_test.is_thread(), false);
        TEST_THIS_EXPECT(bool, task_info_test.is_enabled(), true);
        TEST_THIS_EXPECT(String, task_info_test.name(), "api-sys-test");
        TEST_THIS_EXPECT(bool, task_info_test.memory_size()>0, true);
        TEST_THIS_EXPECT(bool, task_info_test.heap_size()>0, true);
        TEST_THIS_EXPECT(bool, task_info_test.stack_size()>0, true);
        TEST_THIS_EXPECT(bool, task_test.count_total()<task_test.count_free(), false);
        TEST_THIS_EXPECT(bool, task_test.count_total() > 0, true);

        task_test.get_next(task_info_test);
        task_info_test = task_test.get_info(task_test.id());
        TEST_THIS_EXPECT_NOT(int, id_test, task_info_test.id());
    }
    return result;
}
void * TaskTest::thread_1(u32 wait_time){
    int counter = 0;
    u16 test_count ;
    while( !stop_threads ){
        counter++;
        wait(Microseconds(wait_time*1000));
        //add vector stress test from api-var-test(make it more difficult)
        Vector<int> vector_test;
        u16 vector_size;
        if(vector_test.size()!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }
        test_count = rand()&0x3ff;
        vector_size = test_count + 1;
        for (u16 i =0; i<vector_size; i++){
            vector_test.push_back(i);
            vector_test.insert(
                     Vector<int>::Position(i),
                     vector_size-i
                     );
            if(vector_test.count()!=(u16)(i*2+2)){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                thread_result = false;
                break;
            }
        }
        vector_test.shrink_to_fit();
        for (u16 i = 0; i < vector_size*2; i++){
            if(i < vector_size){
                if(vector_test[i]!=(vector_size-i)){
                    print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                    thread_result = false;
                    break;
                }
            }else{
                if(vector_test[i]!=(i - vector_size)){
                    print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                    thread_result = false;
                    break;
                }
            }
        }
        vector_test.free();
    }
    return &thread_result;
}
