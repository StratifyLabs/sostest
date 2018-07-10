#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>
#include "TaskTest.hpp"
TaskTest::TaskTest() : Test("sys::Task"){

}

/*@brief api test for sys/Signal use "api-sys-test -task -api"
 * TaskInfo
 * Task
 * not writed test
 * TaskInfo
 * TaskInfo,TaskInfo,invalid,is_valid,pid,id,thread_id,timer,is_active,priority,priority_ceiling,
 * is_thread,is_enabled,name,memory_size,heap_size,stack_size,print_header,print
 * Task
 *  Task count_total(),count_free,set_id,id,get_next,get_info,get_attr,print
 */
bool TaskTest::execute_class_api_case(){
    bool result = true;
    TaskInfo  task_info_test;
    Task task_test;
    Thread thread_test;
    u32 pid_test;
    int prio_test = 9;
    task_test.set_id((int)Thread::self());
    if(task_test.id() != (int)Thread::self()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    task_info_test = task_test.get_info(task_test.id());
    if(task_info_test.id() != (int)Thread::self()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    pid_test = task_info_test.pid();
    if(!task_info_test.is_active()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    Sched::set_scheduler(Sched::get_pid(),Sched::RR,prio_test);
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
    if( memcmp(task_info_test.name(), "api-sys-test", strlen(task_info_test.name())) != 0 ){
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
    task_test.get_next(task_info_test);
    task_info_test = task_test.get_info(task_test.id());
    if(pid_test == task_info_test.pid()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        print_case_message("pid_test %d == %d",pid_test,task_info_test.pid());
        result = false;
    }
    return result;
}

/*@brief stress test for sys/Signal use "api-sys-test -task -stress"
 */
bool TaskTest::execute_class_stress_case(){
    bool result = true;

    return result;
}

/*@brief performance test for sys/Signal use "api-sys-test -task -performance"
 */
bool TaskTest::execute_class_performance_case(){
    bool result = true;

    return result;
}
