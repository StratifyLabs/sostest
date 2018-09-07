#include "CoreTest.hpp"
static const u32 MIN_OSC_FREQ = 1000;
static const u32 MAX_OSC_FREQ = 25000000;
static const u32 MIN_CPU_FREQ = 1000000;
static const u32 MAX_CPU_FREQ = 1000000000;
static const u32 MIN_PERIPH_FREQ = 1000000;
static const u32 MAX_PERIPH_FREQ = 32000000;
static const u8 MCU_UART_PORTS = 8;
CoreTest::CoreTest() : Test("hal::Core"){

}

/*
 * what return PeriphObject::open
 * */
bool CoreTest::execute_class_api_case(){
    bool result = true;
    core_info_t core_info;
    Core core(0);
    //first get info
    //validate info
    SerialNumber serial;

    serial.get();
    String serial_get_str;
    String serial_str;
    serial_get_str = serial.to_string();
    if(core.open() >=0){
        core.get_info(core_info);
        mcu_board_config_t mcu_board_config;
        core.get_mcu_board_config(mcu_board_config);
        if((mcu_board_config.core_osc_freq < MIN_OSC_FREQ) || \
                (mcu_board_config.core_osc_freq > MAX_OSC_FREQ)){
            print_case_message("core_osc_freq %lu",mcu_board_config.core_osc_freq);
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        if(mcu_board_config.core_cpu_freq <MIN_CPU_FREQ || \
                mcu_board_config.core_cpu_freq >MAX_CPU_FREQ){
            print_case_message("core_cpu_freq %lu",mcu_board_config.core_cpu_freq);
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        if(mcu_board_config.core_periph_freq < MIN_PERIPH_FREQ || \
                mcu_board_config.core_periph_freq >MAX_PERIPH_FREQ){
            print_case_message("core_periph_freq %lu",mcu_board_config.core_periph_freq);
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        if(mcu_board_config.usb_max_packet_zero ==0 ){
            print_case_message("usb_max_packet_zero %lu",mcu_board_config.usb_max_packet_zero);
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        if(mcu_board_config.debug_uart_port > MCU_UART_PORTS){
            print_case_message("debug_uart_port %d",mcu_board_config.debug_uart_port);
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        u32 osc_freq_temp = mcu_board_config.core_cpu_freq;
        core.set_clock_divide(2);
        core.get_mcu_board_config(mcu_board_config);
        if(osc_freq_temp != (2 * mcu_board_config.core_cpu_freq)){
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        core.set_clock_divide(1);
        CoreInfo core_info_c(core_info);
        serial = core_info_c.serial_number();
        serial_str = serial.to_string();
        print_case_message("serial %s",serial_str.c_str());
        if(memcmp(serial_str.c_str(),serial_get_str.c_str(),serial_str.size())){
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        if(core_info_c.o_flags() != mcu_board_config.o_flags){
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
/*        print_case_message("events 0x%08x",core_info_c.o_events());
        String api_version(core_info_c.api_version());
        print_case_message("api version %s",api_version.c_str());
        String delim;
        api_version.get_delimited_data(delim,3,'.');
        print_case_message("api version %s",delim.c_str());
        print_case_message("git hash %s",core_info_c.api_git_hash());*/
        core.close();
    }else{
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    return result;
}
