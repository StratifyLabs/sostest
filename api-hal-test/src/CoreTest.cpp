#include "CoreTest.hpp"

CoreTest::CoreTest() : Test("hal::Core"){

}

/*
 * what return PeriphObject::open
 * */
bool CoreTest::execute_class_api_case(){
    bool result = true;
    core_info_t core_info;
    CoreInfo core_info_class;
    Core core(0);

    //first get info
    //validate info

    SerialNumber serial;
    serial.get();
    String serial_get_str;
    String serial_str;
    serial_get_str = serial.to_string();
    print_case_message("serial get %s",serial_get_str.c_str());
    if(core.open() >=0){
        core.get_info(core_info);
        mcu_board_config_t mcu_board_config;
        core.get_mcu_board_config(mcu_board_config);
        print_case_message("core_osc_freq %lu",mcu_board_config.core_osc_freq);
        print_case_message("core_cpu_freq %lu",mcu_board_config.core_cpu_freq);
        print_case_message("core_periph_freq %lu",mcu_board_config.core_periph_freq);
        print_case_message("usb_max_packet_zero %lu",mcu_board_config.usb_max_packet_zero);
        print_case_message("o_flags %lu",mcu_board_config.o_flags);
        print_case_message("debug_uart_port %d",mcu_board_config.debug_uart_port);
        print_case_message("o_mcu_debug %lu",mcu_board_config.o_mcu_debug);
        core.set_clock_divide(2);
        core.get_mcu_board_config(mcu_board_config);
        print_case_message("core_cpu_freq %lu",mcu_board_config.core_cpu_freq);
        core.set_clock_divide(1);
        core.get_mcu_board_config(mcu_board_config);
        print_case_message("core_cpu_freq %lu",mcu_board_config.core_cpu_freq);
        CoreInfo core_info_c(core_info);
        serial = core_info_c.serial_number();
        serial_str = serial.to_string();
        print_case_message("serial %s",serial_str.c_str());
        if(serial_str!=serial_get_str){
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            result = false;
        }
        print_case_message("events 0x%08x",core_info_c.o_events());
        print_case_message("flags 0x%08x",core_info_c.o_flags());
        core.close();
    }else{
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    return result;
}
