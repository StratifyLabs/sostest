#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "MessengerTest.hpp"
MessengerTest::MessengerTest() : Test("sys::Messenger"){
	Data::reclaim_heap_space();
}
static void rand_string_value(u16 size,String & string);
static u8 pseudo_rnd_u8() ;
static volatile bool stop_threads = false;
static bool thread_result = true;

/*@brief api test for sys/messenger use "api-sys-test -messenger -api"
 */
bool MessengerTest::execute_class_api_case(){
    bool result = true;
    String str_test;
    Son son_test;
    char device[] = "/dev/fifo";
    char son_name[] = "/app/ram/uno.son";
    u16 max_messege_size = 16;
    int read_channel,write_channel;
    u16 time_out = 100;
    File file_test;
    Messenger messenger_test(2048);
    read_channel = 1;
    write_channel = 2;
    if(messenger_test.start(device,read_channel,write_channel)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(messenger_test.write_channel()!=write_channel){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(messenger_test.read_channel()!=read_channel){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    int ret;
    ret = son_test.create(son_name);
    if(ret != 0){
        print_case_message("Failed %s:%d:%d", __PRETTY_FUNCTION__, __LINE__,ret);
        result = false;
    }
    son_test.open_object("");
    son_test.write("name", "Stratify"); //create a string
    son_test.close_object(); //close root ""
    son_test.close();
	 String buffer;
    son_test.open_read(son_name); //opens read-only
	 buffer = son_test.read_string("name");
	 print_case_message("name %s",buffer.cstring());
    son_test.close();

    //buffer holds "Stratify"

    print_case_message("is running %d",messenger_test.is_running());
    print_case_message("fileno %d", messenger_test.fileno());
    print_case_message("max messege size %d", messenger_test.max_message_size());
    print_case_message("timeout size %d", messenger_test.timeout());

    messenger_test.set_max_message_size(max_messege_size);
    if(messenger_test.max_message_size()!=max_messege_size){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    messenger_test.set_timeout(time_out);
    if(messenger_test.timeout()!=time_out){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if(messenger_test.send_message(son_test)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    messenger_test.stop();
    return result;
}

/*@brief stress test for sys/messenger use "api-sys-test -messenger -stress"
 */
bool MessengerTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/messenger use "api-sys-test -messenger -performance"
 */
bool MessengerTest::execute_class_performance_case(){
    bool result = true;
    return result;
}
void * MessengerTest::thread_1(u32 wait_time){
    while( !stop_threads ){
    }
    return &thread_result;
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
static u8 pseudo_rnd_u8() {
    u16 bit;
    static u16 ps_rnd = 1;
    bit = POLINOM_8BIT(ps_rnd);
    ps_rnd = ((ps_rnd << 1) + bit);
    ps_rnd &= 0xFF;
    return ps_rnd;
}
