#include "MicroTimeTest.hpp"
#include "sapi/chrono.hpp"

#include <sapi/sys.hpp>

MicroTimeTest::MicroTimeTest() : Test("chrono::MicroTime"){

}

/*! \details test "api" a chrono::MicroTime
 *  constructors,+=(MicroTime,const,value),
 *  -=(MicroTime,const,value),set,from
 *  invalid, is_valid,
 * @return false if some test failed
 */

bool MicroTimeTest::execute_class_api_case(){
    bool result = true;
    MicroTime micro_time;
    u32 value_u;
    u32 check_value;
    if(micro_time.microseconds()!=micro_time.usec()||
            micro_time.msec()!=micro_time.milliseconds()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    check_value =1000;
    micro_time.set_microseconds(check_value);
    if(micro_time.microseconds()!=check_value||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //operator +=
    MicroTime micro_time_add(499);
    check_value += 499;
    micro_time+=micro_time_add;
    if(micro_time.microseconds()!=check_value||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    check_value += 1;
    micro_time += 1;
    if(micro_time.microseconds()!=check_value||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    value_u = 500;
    check_value += value_u;
    micro_time += value_u;
    if(micro_time.microseconds()!=check_value ||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //operator -=
    check_value -= 499;
    micro_time -=micro_time_add;
    if(micro_time.microseconds()!=check_value||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    check_value -= 1;
    micro_time -= 1;
    if(micro_time.microseconds()!=check_value||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    value_u = 500;
    check_value -= value_u;
    micro_time -= value_u;
    if(micro_time.microseconds()!=check_value ||
            micro_time.milliseconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //set
    //us
    check_value = 1000000;
    micro_time.set_microseconds(1000000);
    if(micro_time.microseconds()!=check_value ||
       micro_time.milliseconds()!=check_value/1000||
       micro_time.seconds()!=check_value/1000000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    check_value = rand()&0xffff;
    micro_time.set_microseconds(check_value );
    if(micro_time.microseconds()!=check_value ||
       micro_time.milliseconds()!=check_value/1000||
       micro_time.seconds()!=check_value/1000000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //ms
    check_value = 1000;
    micro_time.set_milliseconds(1000);
    if(micro_time.microseconds()/1000!=check_value ||
       micro_time.milliseconds()!=check_value||
       micro_time.seconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    check_value = rand()&0xffff;
    micro_time.set_milliseconds(check_value );
    if(micro_time.microseconds()/1000!=check_value ||
       micro_time.milliseconds()!=check_value||
       micro_time.seconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //second
    check_value = 12;
    micro_time.set_sec(12);
    if(micro_time.microseconds()/1000000!=check_value||
       micro_time.milliseconds()/1000!=check_value||
       micro_time.seconds()!=check_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    check_value = rand()&0xfff;
    micro_time.set_sec(check_value);
    if(micro_time.microseconds()/1000000!=check_value||
       micro_time.milliseconds()/1000!=check_value||
       micro_time.seconds()!=check_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //from
    MicroTime time_from;
    time_from = MicroTime::from_microseconds(check_value);
    if(time_from.microseconds()!=check_value ||
       time_from.milliseconds()!=check_value/1000||
       time_from.seconds()!=check_value/1000000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    time_from = MicroTime::from_usec(check_value);
    if(time_from.microseconds()!=check_value ||
       time_from.milliseconds()!=check_value/1000||
       time_from.seconds()!=check_value/1000000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    time_from = MicroTime::from_milliseconds(check_value);
    if(time_from.microseconds()/1000!=check_value ||
       time_from.milliseconds()!=check_value||
       time_from.seconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    time_from = MicroTime::from_msec(check_value);
    if(time_from.microseconds()/1000!=check_value ||
       time_from.milliseconds()!=check_value||
       time_from.seconds()!=check_value/1000){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    time_from = MicroTime::from_sec(check_value);
    if(time_from.microseconds()/1000000!=check_value ||
       time_from.milliseconds()/1000!=check_value||
       time_from.seconds()!=check_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    time_from = MicroTime::from_seconds(check_value);
    if(time_from.microseconds()/1000000!=check_value ||
       time_from.milliseconds()/1000!=check_value||
       time_from.seconds()!=check_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    // micro_time_t
    time_from = MicroTime::from_microseconds(check_value);
    micro_time_t  micro_time_test;
    micro_time_test = time_from.microseconds();
    if(micro_time_test != check_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    micro_time_test = time_from.usec();
    if(micro_time_test != check_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!time_from.is_valid()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    time_from = MicroTime::invalid();
    if(time_from.is_valid()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    return result;
}
