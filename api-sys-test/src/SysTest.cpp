
#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>
#include "SysTest.hpp"

SysTest::SysTest() : Test("sys::Test"){

}
/*@brief api test for sys/Sys use "api-sys-test -sys -api"
 * Sys
 * launch,free_ram
 * not writed test
 * ,reclaim_ram,assign_zero_sum32,verify_zero_sum32,
 * get_version,get_kernel_version,powerdown,hibernate,request,reset,
 * get_board_config,
 * open,get_info,get_23_info,get_26_info,get_taskattr,current_task,
 * set_current_task,get_id,redirect_stdout,redirect_stdin,redirect_stderr,
 *
 * not tested on
 *
 */
bool SysTest::execute_class_api_case(){
    bool result;
    result = true;
    //start fake
    if(Sys::launch("/app/flash/fake",0,"orphan",0,Sys::LAUNCH_RAM_SIZE_DEFAULT,0,0)>=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //start preinstalled app launchslave
    if(Sys::launch("/app/flash/launchslave",0,"orphan",0,Sys::LAUNCH_RAM_SIZE_DEFAULT,0,0)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    Timer::wait_microseconds(10000);
    if(Sys::free_ram("/app/flash/launchslave",0)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    return result;
}
/*@brief stress test for sys/Sys use "api-sys-test -sys -stress"
 *
 * not writed test
 *
 * not tested on
 */
bool SysTest::execute_class_stress_case(){
    bool result;
    result = true;
    const u32 itterate_num = 100;
    for(u32 i=0;i<itterate_num;i++){
        //launch apps
        //stop apps
    }
    return result;

}

/*@brief performsnce test for sys/Sys use "api-sys-test -sys -performance"
 *
 * not writed test
 *
 * not tested on
 */
bool SysTest::execute_class_performance_case(){
    bool result;
    result = true;

    return result;

}
