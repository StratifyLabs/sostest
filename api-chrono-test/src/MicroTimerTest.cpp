#include "MicroTimerTest.hpp"
#include "sapi/chrono.hpp"
#include <sapi/sys.hpp>
MicroTimerTest::MicroTimerTest() : Test("chrono::MicroTimer")
{

}
/*! \details test "api" a chrono::MicroTimer
 *  constructors,start,resume,wait,
 * @return false if some test failed
 */

bool MicroTimerTest::execute_class_api_case(){
    bool result = true;
    const u32 delay_time_usec = 100;
    const u32 delay_time_msec = 10;
    const u32 delay_time_sec = 1;
    MicroTimer timer_count;
    MicroTime time_test;
    u32 time_usec,time_msec,time_sec;
    timer_count.start();
    timer_count.wait_usec(delay_time_usec);
    //for incredible add resume
    timer_count.resume();
    if (timer_count.calc_usec() < delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if (timer_count.calc_usec() > 2*delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    timer_count.stop();
    time_usec = timer_count.calc_usec();
    time_test = timer_count.calc_value();
    if(time_test.microseconds() != time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if (time_usec < delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if (time_usec > 2*delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    timer_count.stop();
    if(time_usec != timer_count.calc_usec()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_usec()!=timer_count.usec()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.reset();
    if(timer_count.calc_usec()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.wait_usec(delay_time_usec);
    if(timer_count.calc_usec()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.restart();
    timer_count.wait_usec(delay_time_usec);
    timer_count.stop();
    time_usec = timer_count.calc_usec();
    if (time_usec < delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.stop();
    if(time_usec != timer_count.calc_usec()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.resume();
    timer_count.wait_usec(delay_time_usec);
    time_usec = timer_count.calc_usec();
    if (time_usec < 2*delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.restart();
    if(timer_count.calc_usec() > delay_time_usec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.is_stopped()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!timer_count.is_started()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!timer_count.is_running()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.is_reset()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //after stop
    timer_count.stop();
    if(!timer_count.is_stopped()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //has been started
    if(!timer_count.is_started()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.is_running()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.is_reset()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //after reset
    timer_count.reset();
    if(timer_count.calc_usec()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_msec()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_sec()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!timer_count.is_stopped()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //has been started
    if(timer_count.is_started()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.is_running()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!timer_count.is_reset()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //msec
    timer_count.restart();
    timer_count.wait_msec(delay_time_msec);
    timer_count.stop();
    time_msec = timer_count.calc_msec();
    time_test = timer_count.calc_value();
    if(time_test.milliseconds() != time_msec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if(timer_count.calc_msec()<delay_time_msec||
       timer_count.calc_msec()>(delay_time_msec*2)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_msec()!=(timer_count.calc_usec()/1000)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.resume();
    timer_count.wait_msec(delay_time_msec);
    if(timer_count.calc_msec()<(2*delay_time_msec)){
        print_case_message("micro calc_masec %lu ",timer_count.calc_msec());
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_msec()!=(timer_count.calc_usec()/1000)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //sec
    timer_count.restart();
    timer_count.wait_sec(delay_time_sec);
    timer_count.stop();
    time_sec = timer_count.calc_sec();
    time_test = timer_count.calc_value();
    if(time_test.seconds() != time_sec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_sec() != delay_time_sec){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_sec()!=(timer_count.calc_msec()/1000)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_msec()!=(timer_count.calc_usec()/1000)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    timer_count.resume();
    timer_count.wait_sec(delay_time_sec);
    if(timer_count.calc_sec()<(2*delay_time_sec)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_sec()!=(timer_count.calc_msec()/1000)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(timer_count.calc_msec()!=(timer_count.calc_usec()/1000)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;
}

bool MicroTimerTest::execute_class_stress_case(){
    bool result = true;
    const u32 delay_time_usec = 200;
    u32 delay_time = 100;
    MicroTimer timer_count;
    u32 time_usec,temp_usec;
    time_usec = 0;
    u32 max_delta_time;
    max_delta_time = 0;
    timer_count.restart();
    for (u16 i = 1;i<10000;i++){
        timer_count.wait_usec(delay_time_usec);
        temp_usec = timer_count.calc_usec();
        if((temp_usec - (time_usec + delay_time_usec))>max_delta_time){
            max_delta_time = temp_usec - (time_usec + delay_time_usec);
        }
        if(temp_usec < (time_usec + delay_time_usec)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }

        if(temp_usec > (time_usec + 2*delay_time_usec)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            print_case_message("Failed %lu:%lu:%lu", temp_usec , (time_usec + 2*delay_time),i);
            break;
        }
        time_usec = timer_count.calc_usec();
    }
    time_usec = 0;
    timer_count.restart();
    print_case_message_with_key("max delta time","%lu",max_delta_time);
    max_delta_time = 0;
    for (u16 i = 1;i<1000;i++){
        delay_time = (rand()&0xffff) + 100;
        timer_count.wait_usec(delay_time);
        temp_usec = timer_count.calc_usec();
        if((temp_usec - (time_usec + delay_time))>max_delta_time){
            max_delta_time = temp_usec - (time_usec + delay_time);
        }
        if(temp_usec < time_usec + delay_time){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        if((temp_usec > (time_usec + 2*delay_time))/* ||
           (temp_usec > (time_usec + delay_time + 400))*/){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            print_case_message("Failed %lu:%lu:%lu:%lu", delay_time,timer_count.calc_usec(), (time_usec + 2*delay_time),i);
            result = false;
            break;
        }
        time_usec = timer_count.calc_usec();
    }
    print_case_message_with_key("max delta time","%lu",max_delta_time);
    return result;
}
