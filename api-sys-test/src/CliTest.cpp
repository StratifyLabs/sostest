#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include "CliTest.hpp"
CliTest::CliTest() : Test("sys::Cli"){

}
static volatile bool stop_threads = false;
static bool thread_result = true;

/*@brief api test for sys/cli use "api-sys-test -cli -api"
 * 	Cli(int argc, char * argv[])
 * var::String at(u16 value)
 * 	void set_publisher(const char * publisher)
 * const char * publisher()
 * const char * version()
 * const char * name()
 * const char * path()
 * int count()int size()
 *  void set_case_sensitive(bool value = true)
 *  bool is_case_senstive()
 * bool is_option(const char * value)
 * int get_option_value(const char * option)

 * not tested on
 * 	void handle_version() const;

 * not writed test


 * mcu_pin_t pin_at(u16 value)
 * int value_at(u16 value)
 * var::String get_option_argument(const char * option)
 * int get_option_hex_value(const char * option)
 * mcu_pin_t get_option_pin(const char * option)
 * bool handle_uart(hal::UartAttr & attr)
 * bool handle_i2c(hal::I2CAttr & attr)
 * @warning
 * fatal error if use cli_test.at(value) for value more the arg num
 * suspend without trace info if call cli_test.handle_version();
 *
 */
bool CliTest::execute_class_api_case(){
    bool result = true;
    char arg_path[] = "path/test";
    char arg_param1[] = "-cli";
    char arg_param2[] = "-API";
    char arg_param3[] = "-Stratify";
    char arg_param4[] = "16";
    char arg_param5[] = "-SoS";
    char arg_param6[] = "0x0e";
    char arg_param7[] = "-opt_Arg";
    char arg_param8[] = "last.test";
    char arg_param9[] = "-v";

    char publisher[] = "Stratify Labs, Inc";
    char* arg_str[10] ;
    String temp_string;
    arg_str[0] = arg_path;
    arg_str[1] = arg_param1;
    arg_str[2] = arg_param2;
    arg_str[3] = arg_param3;
    arg_str[4] = arg_param4;
    arg_str[5] = arg_param5;
    arg_str[6] = arg_param6;
    arg_str[7] = arg_param7;
    arg_str[8] = arg_param8;
    arg_str[9] = arg_param9;

    u8 arg_number = 10;
    Cli cli_test(arg_number,arg_str);
    if(arg_number != cli_test.size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(strlen(cli_test.version())<2){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

//    cli_test.handle_version();    //suspend if use it
    u8 name_start = (u8)(sizeof (arg_path) - strlen(cli_test.name())-1);
    if(memcmp(cli_test.name(),&arg_path[name_start],strlen(cli_test.name()))&&
            (strlen(cli_test.name())>0)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(memcmp(cli_test.path(),arg_path,sizeof (arg_path))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    temp_string = cli_test.at(0);
    if(memcmp(temp_string.c_str(),arg_path,sizeof (arg_path))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    temp_string = cli_test.at(1);
    if(memcmp(temp_string.c_str(),arg_param1,sizeof (arg_param1))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    temp_string = cli_test.at(2);
    if(memcmp(temp_string.c_str(),arg_param2,sizeof (arg_param2))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    temp_string = cli_test.at(3);
    if(memcmp(temp_string.c_str(),arg_param3,sizeof (arg_param3))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    cli_test.set_publisher(publisher);
    if(memcmp(cli_test.publisher(),publisher,sizeof (publisher))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    cli_test.set_case_sensitive(true);
    if(!cli_test.is_case_senstive()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    cli_test.set_case_sensitive(false);
    if(cli_test.is_case_senstive()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    temp_string = cli_test.at(2);
    temp_string.to_lower();
    if(!cli_test.is_option(temp_string.c_str())){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    cli_test.set_case_sensitive(true);
    if(cli_test.is_option(temp_string.c_str())){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    u8 option_value_number;
    int option_value;
    option_value_number = 3;
    temp_string = cli_test.at(option_value_number);
    option_value = cli_test.get_option_value(temp_string.c_str());
    temp_string = cli_test.at(option_value_number+1);
    if(option_value != temp_string.atoi()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    option_value_number = 5;
    temp_string = cli_test.at(option_value_number);
    option_value = cli_test.get_option_hex_value(temp_string.c_str());
    temp_string = cli_test.at(option_value_number+1);
    if(option_value != 14){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    option_value_number = 7;
    temp_string = cli_test.at(option_value_number);
    temp_string = cli_test.get_option_argument(temp_string.c_str());

    if(memcmp(temp_string.c_str(),arg_str[option_value_number+1],strlen(arg_str[option_value_number+1]))){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    return result;
}

/*@brief stress test for sys/cli use "api-sys-test -cli -stress"
 */
bool CliTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/Cli use "api-sys-test -cli -performance"
 */
bool CliTest::execute_class_performance_case(){
    bool result = true;

    return result;
}
void * CliTest::thread_1(u32 wait_time){
    int counter = 0;
    u16 test_count ;
    while( !stop_threads ){
        counter++;
        Timer::wait_microseconds(wait_time*1000);
        //add vector stress test from api-var-test(make it more difficult)
        Vector<int> vector_test;
        u16 vector_size;
        if(vector_test.size()!=0){
            thread_result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }
        test_count = rand()&0x3ff;
        vector_size = test_count + 1;
        for (u16 i =0; i<vector_size; i++){
            vector_test.push_back(i);
            vector_test.insert(i,vector_size-i);
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
