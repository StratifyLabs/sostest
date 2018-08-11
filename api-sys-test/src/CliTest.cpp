#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include "CliTest.hpp"
CliTest::CliTest() : Test("sys::Cli"){

}
static void rand_string_value(u16 size,String & string);
u8 pseudo_rnd_u8() ;

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
 * var::String get_option_argument(const char * option)
 * int get_option_hex_value(const char * option)

 * not tested on
 * 	void handle_version() const;

 * not writed test
 * mcu_pin_t pin_at(u16 value)
 * int value_at(u16 value)
 * mcu_pin_t get_option_pin(const char * option)
 * bool handle_uart(hal::UartAttr & attr)
 * bool handle_i2c(hal::I2CAttr & attr)
 *
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
    int itterate = 256;
    String temp_string,name_string;
    const u8 MAX_NAME_LEN = 128;
    u8 name_len;
    String arg_path("path/");
    String arg_name("test");
    String arg_path_and_name = arg_path;
    arg_path_and_name +=  arg_name;
    String arg_param1("-cli");
    String arg_param2("-API");
    String arg_param3("-Stratify");
    const u8 STR_INT_NUMBER = 5;
    String arg_param4[STR_INT_NUMBER];
    arg_param4[0] = "1";
    arg_param4[1] = "-1";
    arg_param4[2] = "1234567890";
    arg_param4[3] = "9999999999";
    arg_param4[4] = "0999999999";
    String arg_param5("-SoS");
    String arg_param6[STR_INT_NUMBER];
    arg_param6[0] = "0x0e";
    arg_param6[1] = "0x00000000";
    arg_param6[2] = "0xffff";
    arg_param6[3] = "0x1000";
    arg_param6[4] = "0xabcd";
    int hex_value[STR_INT_NUMBER]  = {14,0,65535,4096,43981};
    String arg_param7("-opt_Arg");
    String arg_param8("last.test");
    String arg_param9("-v");
    String publisher("Stratify Labs, Inc");
    String defis("-");
    char* arg_str[10] ;
    for (u16 i=0;i<itterate;i++){
        name_len = pseudo_rnd_u8();//zero will be inluded
        name_len = name_len%MAX_NAME_LEN;
        rand_string_value(name_len,arg_path);
        rand_string_value(name_len,arg_name);
        arg_path_and_name = arg_path;
        arg_path_and_name += '/';
        arg_path_and_name +=  arg_name;
        arg_str[0] = (char*)arg_path_and_name.c_str();
        rand_string_value(name_len,arg_param1);
        arg_param1.insert(0,defis.c_str());
        arg_str[1] = (char*)arg_param1.c_str();
        rand_string_value(name_len,arg_param2);
        arg_param2.insert(0,defis.c_str());
        arg_str[2] = (char*)arg_param2.c_str();
        rand_string_value(name_len,arg_param3);
        arg_param3.insert(0,defis.c_str());
        arg_str[3] = (char*)arg_param3.c_str();
        arg_str[4] = (char*)arg_param4[i%STR_INT_NUMBER].c_str();
        arg_str[5] = (char*)arg_param5.c_str();
        arg_str[6] = (char*)arg_param6[i%STR_INT_NUMBER].c_str();
        arg_str[7] = (char*)arg_param7.c_str();
        arg_str[8] = (char*)arg_param8.c_str();
        arg_str[9] = (char*)arg_param9.c_str();
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

        if(memcmp(cli_test.name(),arg_name.c_str(),strlen(cli_test.name()))&&
                (strlen(cli_test.name())>0)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        if(memcmp(cli_test.path(),arg_path_and_name.c_str(),strlen(cli_test.path()))){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            print_case_message("Failed %s:%s", cli_test.path(),arg_path.c_str());
            result = false;
        }
        temp_string = cli_test.at(0);
        if(memcmp(temp_string.c_str(),arg_path_and_name.c_str(),strlen(cli_test.path()))){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        temp_string = cli_test.at(1);
        if(memcmp(temp_string.c_str(),arg_param1.c_str(),arg_param1.length())){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        temp_string = cli_test.at(2);
        if(memcmp(temp_string.c_str(),arg_param2.c_str(),arg_param2.length())){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        temp_string = cli_test.at(3);
        if(memcmp(temp_string.c_str(),arg_param3.c_str(),arg_param3.length())){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        cli_test.set_publisher(publisher.c_str());
        if(memcmp(cli_test.publisher(),publisher.c_str(),publisher.length())){
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
        temp_string.to_upper();
        if(!cli_test.is_option(temp_string.c_str())){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        cli_test.set_case_sensitive(true);
        if(cli_test.is_option(temp_string.c_str()) && name_len){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        u8 option_value_number;
        int option_value;
        option_value_number = 3;
        temp_string = cli_test.at(option_value_number);
        option_value = cli_test.get_option_value(temp_string.c_str());
        if((option_value != arg_param4[i%STR_INT_NUMBER].atoi()) && name_len){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        option_value_number = 5;
        temp_string = cli_test.at(option_value_number);
        option_value = cli_test.get_option_hex_value(temp_string.c_str());
        if(option_value != hex_value[i%STR_INT_NUMBER]){
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
        if(!result){
            break;
        }
    }
    return result;
}

/*@brief performance test for sys/Cli use "api-sys-test -cli -performance"
 */
bool CliTest::execute_class_performance_case(){
    bool result = true;
    int itterate = 256;
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
    for (u16 i=0;i<itterate;i++){
        Cli cli_test(arg_number,arg_str);
        if(arg_number != cli_test.size()){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        if(strlen(cli_test.version())<2){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }

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
        if(!result){
            break;
        }
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
#define BIT(n) (1<<n)
#define POLINOM_8BIT(ps_rnd) (((ps_rnd & BIT(7)) >> 7)^((ps_rnd & BIT(3)) >> 3)^((ps_rnd & BIT(2)) >> 2)^((ps_rnd & BIT(1)) >> 1)^1)
u8 pseudo_rnd_u8() {
    u16 bit;
    static u16 ps_rnd = 1;
    bit = POLINOM_8BIT(ps_rnd);
    ps_rnd = ((ps_rnd << 1) + bit);
    ps_rnd &= 0xFF;
    return ps_rnd;
}
