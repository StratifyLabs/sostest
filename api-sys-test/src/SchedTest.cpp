#include <sapi/sys.hpp>

#include "SchedTest.hpp"
SchedTest::SchedTest():Test("sys::Sched")
{

}
/*@brief test api for sched
 * RR and FIFO policy
 * get_priority_max,get_priority_min,
 * get_pid,set_scheduler,get_priority
 * get_rr_interval
 * */
bool SchedTest::execute_class_api_case(){
    bool result = true;
    pid_t procces_id;
    int rr_priority_max,rr_priority_min;
    int fifo_priority_max,fifo_priority_min;
    int other_priority_max,other_priority_min;
    const int priority_rr = 5;
    const int priority_fifo = 4;
//    const int priority_other = 3;
    int priority;
    int rr_interval;
    Sched sched_in;
    rr_priority_max = Sched::get_priority_max(Sched::RR);
    if(rr_priority_max<=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    rr_priority_min = Sched::get_priority_min(Sched::RR);
    if(rr_priority_min<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    fifo_priority_max = Sched::get_priority_max(Sched::FIFO);
    if(fifo_priority_max<=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    fifo_priority_min = Sched::get_priority_min(Sched::FIFO);
    if(fifo_priority_min<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    other_priority_max = Sched::get_priority_max(Sched::OTHER);
    if(other_priority_max<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    other_priority_min = Sched::get_priority_min(Sched::OTHER);
    if(other_priority_min<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //get proccess id
    procces_id = sched_in.get_pid();
    if(procces_id<=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    sched_in.yield();
    if (sched_in.set_scheduler(procces_id, Sched::RR, priority_rr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    sched_in.yield();
    priority = sched_in.get_priority(procces_id);
    if((priority!=priority_rr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    rr_interval = sched_in.get_rr_interval(procces_id);
    if(rr_interval <= 0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if (sched_in.set_scheduler(procces_id, Sched::FIFO, priority_fifo)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    sched_in.yield();
    priority = sched_in.get_priority(procces_id);
    if((priority!=priority_fifo)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    rr_interval = sched_in.get_rr_interval(procces_id);
    if(rr_interval <= 0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //imposible value
    //max+1
    if (!sched_in.set_scheduler(procces_id, Sched::FIFO, fifo_priority_max+1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
        priority = sched_in.get_priority(procces_id);
        if((priority!=rr_priority_max)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    sched_in.yield();
    if (!sched_in.set_scheduler(procces_id, Sched::RR, rr_priority_max+1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
        priority = sched_in.get_priority(procces_id);
        if((priority!=rr_priority_max)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }

    }
    sched_in.yield();

    //min -1
    if (!sched_in.set_scheduler(procces_id, Sched::FIFO, fifo_priority_min-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
        priority = sched_in.get_priority(procces_id);
        if((priority!=fifo_priority_min)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    sched_in.yield();
    if (!sched_in.set_scheduler(procces_id, Sched::RR, rr_priority_min-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
        priority = sched_in.get_priority(procces_id);
        if((priority!=rr_priority_min)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }

    }


    return result;
}
/* @brief stress test for sys Sched
 */
bool SchedTest::execute_class_stress_case(){
    bool result = true;
    pid_t procces_id;
    int priority_set;
    int priority_get;
    int fifo_priority_min,fifo_priority_max;
    int rr_priority_min,rr_priority_max;
    int rr_interval;
    rr_priority_max = Sched::get_priority_max(Sched::RR);
    fifo_priority_max = Sched::get_priority_max(Sched::FIFO);
    rr_priority_min = Sched::get_priority_min(Sched::RR);
    fifo_priority_min = Sched::get_priority_min(Sched::FIFO);

    const u16 itteration_number = 1000;
    for(u16 i=0; i < itteration_number; i++){
        //get proccess id
        procces_id = Sched::get_pid();
        priority_set = i % fifo_priority_max;
        priority_set = priority_set < fifo_priority_min?fifo_priority_min:priority_set;
        if(procces_id <= 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;

        }
        if (Sched::set_scheduler(procces_id, Sched::FIFO, priority_set)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        Sched::yield();
        priority_get = Sched::get_priority(procces_id);
        if((priority_get!=priority_set)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        rr_interval = Sched::get_rr_interval(procces_id);
        if(rr_interval <= 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        priority_set = i % rr_priority_max;
        priority_set = priority_set < rr_priority_min?rr_priority_min:priority_set;
        if(procces_id <= 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;

        }
        if (Sched::set_scheduler(procces_id, Sched::RR, priority_set)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        Sched::yield();
        priority_get = Sched::get_priority(procces_id);
        if((priority_get!=priority_set)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        rr_interval = Sched::get_rr_interval(procces_id);
        if(rr_interval <= 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    return result;
}
