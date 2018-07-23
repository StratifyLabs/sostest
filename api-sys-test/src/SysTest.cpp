
#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>
#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include "SysTest.hpp"
static void rand_string_value(u16 size,String & string);
static u32 get_id(u32 pid);
SysTest::SysTest() : Test("sys::Test"){

}
/*@brief api test for sys/Sys use "api-sys-test -sys -api"
 * Sys
 * launch,free_ram
 * assign_zero_sum32,verify_zero_sum32,
 * open,get_version,get_kernel_version,get_board_config,
 * get_info,get_23_info,get_26_info,get_taskattr,current_task,
 * set_current_task,get_id,
 * not writed test
 * reclaim_ram,
 * ,powerdown,hibernate,request,reset,
 * redirect_stdout,redirect_stdin,redirect_stderr,
 *
 * not tested on
 *
 */
bool SysTest::execute_class_api_case(){
    bool result;
    String version;
    result = true;
    const u16 buff_size = 50*sizeof(u32);
    u8 temp_buff[buff_size];
    sos_board_config_t config;
    sys_info_t sys_info;
    Sys board;
    //start fake
    if(Sys::launch("/app/flash/fake",nullptr,"orphan",0,Sys::LAUNCH_RAM_SIZE_DEFAULT,nullptr,nullptr)>=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //start preinstalled app launchslave
    if(Sys::launch("/app/flash/launchslave",nullptr,"task_id",0,Sys::LAUNCH_RAM_SIZE_DEFAULT,nullptr,nullptr)<0){
        print_case_message("need to install launchslave app");
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    Timer::wait_microseconds(40000);
    if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    for (u32 i =0;i<buff_size;i++){
       temp_buff[i] = (u8)i;
    }
    Sys::assign_zero_sum32(temp_buff,buff_size);
    if(!Sys::verify_zero_sum32(temp_buff,buff_size)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(Sys::get_version(version)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(Sys::get_kernel_version(version)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(board.open()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(board.get_board_config(config)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(board.get_info(sys_info)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    int current_task;
    current_task = board.current_task();
    if(current_task<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    current_task+=1;
    board.set_current_task(current_task);
    if(board.current_task() != current_task){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    sys_id_t sys_id;
    if(board.get_id(sys_id)!=0){
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
    char exec_path[512];
    char arg_buff[48];
    String temp_str;
    const u32 itterate_num = 64;
    u16 ram_size;
    int pid;
    for(u32 i=0;i<itterate_num;i++){
        //launch apps
        //start preinstalled app launchslave
        rand_string_value(sizeof(arg_buff),temp_str);
        ram_size = rand() & 0xfff;
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_FLASH, ram_size ,nullptr,nullptr);
        if(pid < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        //print_case_message("LAUNCH_OPTIONS_FLASH %d",pid);
        Timer::wait_microseconds(40000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_ROOT, ram_size ,nullptr,nullptr);
        if(pid<0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        //print_case_message("LAUNCH_OPTIONS_ROOT %d",pid);
        Timer::wait_microseconds(40000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_ORPHAN, ram_size ,nullptr,nullptr);
        if(pid < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        //print_case_message("LAUNCH_OPTIONS_ORPHAN %d",pid);
        Timer::wait_microseconds(40000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_UNIQUE_NAMES, ram_size ,nullptr,nullptr);
        if(pid < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        //print_case_message("LAUNCH_OPTIONS_UNIQUE_NAMES %d",pid);
        Timer::wait_microseconds(40000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
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
    result = true;
    char exec_path[512];
    char arg_buff[] = "task";
    String temp_str;
    Task task_test;
    TaskInfo  task_info_test;
    const u32 itterate_num = 64;
    u16 ram_size;
    int pid;
    u32 id;
    for(u32 i=0;i<itterate_num;i++){
        //launch apps
        //start preinstalled app launchslave
        ram_size = 512;
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_FLASH, ram_size ,nullptr,nullptr);
        if(pid < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        id = get_id(pid);
        task_test.set_id(id);
        task_info_test = task_test.get_info(id);
        Timer::wait_microseconds(5000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_ROOT, ram_size ,nullptr,nullptr);
        if(pid<0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        id = get_id(pid);
        task_test.set_id(id);
        task_info_test = task_test.get_info(id);
        Timer::wait_microseconds(5000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_ORPHAN, ram_size ,nullptr,nullptr);
        if(pid < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        id = get_id(pid);
        task_test.set_id(id);
        task_info_test = task_test.get_info(id);
        Timer::wait_microseconds(5000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        pid = Sys::launch("/app/flash/launchslave",exec_path,arg_buff,\
                          Sys::LAUNCH_OPTIONS_UNIQUE_NAMES, ram_size ,nullptr,nullptr);
        id = get_id(pid);
        task_test.set_id(id);
        task_info_test = task_test.get_info(id);

        if(pid < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        Timer::wait_microseconds(5000);
        if(Sys::free_ram("/app/flash/launchslave",nullptr)!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }
        //stop apps
    }
    return result;
}
static void rand_string_value(u16 size,String & string){
    string.clear();
    for (u16 i =0;i<size;i++){
        u8 value;
        value = (u8)(rand()%25);
        value +=97;
        string.append(value);
    }
}
static u32 get_id(u32 pid){
    Task task_test;
    TaskInfo  task_info_test;
    for(u8 i = 255;i;i--){
        task_test.get_next(task_info_test);
        if (task_info_test.pid() == pid){
            return task_info_test.id();
        }

    }
    return 0;
}
