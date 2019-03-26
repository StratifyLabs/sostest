#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/hal.hpp>
#include "UartTest.hpp"
#define USE_TWO_UARTS 0
#if USE_TWO_UARTS
static void * thread_rx(void * args);
static int result = 0;
static bool byte_recv = false;

#endif
static char messege_text[] = "hello_two_aio";
static u32 get_freq(u32 i);
static u32 get_flags_stop_bit(u32 i);
static u32 get_parity(u32 i);
static const u8 MAX_FREQ_TYPES = 12;
UartTest::UartTest() : Test("hal::Uart"){

}


bool UartTest::execute_class_api_case(){
    bool result = true;
    get_uart_count();
    print_case_message("Board has %d Uarts", m_uart_count);
    for(PeriphObject::port_t count = 0; count < m_uart_count; count++){
        Uart uart(count);
        if( execute_uart_api_case(uart) == false ){
            result = false;
        }
    }
#if USE_TWO_UARTS
    Thread uno_thread(1024);
    enum Sched::policy uno_policy = Sched::RR;
    int uno_priority =Sched::get_priority_max(Sched::RR);
    if( uno_thread.create(thread_rx,this,uno_priority,uno_policy) != 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
#endif
    Uart uart_tx(0);
    if( uart_tx.open(Uart::RDWR|Uart::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
        result = false;
    } else {
        UartPinAssignment uart_pin_assignment;
        uart_pin_assignment->tx = mcu_pin(0,9);
        uart_pin_assignment->rx = mcu_pin(0,10);

        UartAttr uart_attr;//not used
        if(uart_tx.set_attr(UART_FLAG_SET_LINE_CODING,115200,8,uart_pin_assignment)!=0){
            print_case_message("Failed %s %d", __FILE__, __LINE__);
            result = false;
        }
#if USE_TWO_UARTS
        while(!byte_recv){

            if(uart_tx.put('w')!=0){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                result = false;
            }
            if(uart_tx.write(messege_text,sizeof(messege_text))!=sizeof(messege_text)){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                result = false;
            }
            Timer::wait_milliseconds(100);
        }
#else
        String message(messege_text);
        {
            char recv_buff[message.size()];
            Aio aio_r(recv_buff, sizeof(messege_text)); //aio uses buf as it's data
            Aio aio_t(messege_text, sizeof(messege_text)); //aio uses buf as it's data
            uart_tx.flush();    //flush before recved packet
            uart_tx.write(aio_t);
            while( !aio_t.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            uart_tx.read(aio_r);
            while( !aio_r.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            if(memcmp(messege_text,recv_buff,sizeof(messege_text))){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                print_case_message("recv %s %s", recv_buff,messege_text);
                result = false;
            }
        }
        {
            char recv_buff[message.size()];
            Aio aio_r(recv_buff, sizeof(messege_text)); //aio uses buf as it's data
            Aio aio_t(messege_text, sizeof(messege_text)); //aio uses buf as it's data
            uart_tx.write(aio_t);
            while( !aio_t.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            uart_tx.read(aio_r);
            while( !aio_r.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            if(memcmp(messege_text,recv_buff,sizeof(messege_text))){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                print_case_message("recv %s %s", recv_buff,messege_text);
                result = false;
            }
        }

        {
            message.append("12");
            char recv_buff[message.size()];
            Aio aio_r(recv_buff, message.size()); //aio uses buf as it's data
            aio_r.set_buf(recv_buff,(u32)message.size());
            uart_tx.write(message.c_str(),message.size());//should be stay untill will write
            uart_tx.read(aio_r);    //flush before recved packet
            while( !aio_r.is_done()){
                Timer::wait_milliseconds(5); //wait for the operation to complete
            }
            if(memcmp(message.c_str(),recv_buff,message.size())){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                print_case_message("recv %s %s", recv_buff,message.c_str());
                result = false;
            }
        }
        {
            message.append("last");
            char recv_buff[message.size()];
            Aio aio_r(recv_buff, message.size()); //aio uses buf as it's data
            aio_r.set_buf(recv_buff,(u32)message.size());
            uart_tx.write(message.c_str(),message.size());//should be stay untill will write
            uart_tx.read(aio_r);    //flush before recved packet
            while( !aio_r.is_done()){
                Timer::wait_milliseconds(5); //wait for the operation to complete
            }
            if(memcmp(message.c_str(),recv_buff,message.size())){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                print_case_message("recv %s %s", recv_buff,message.c_str());
                result = false;
            }
        }
        //set another attr
        if(uart_tx.set_attr(UART_FLAG_SET_LINE_CODING,115200,8,uart_pin_assignment)!=0){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
            result = false;
		}
        if( uart_tx.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
            result = false;
        }

#endif
    }
#if USE_TWO_UARTS
    if(uno_thread.is_running()){
        uno_thread.wait();
    }
#endif

    return result;
}


bool UartTest::execute_class_performance_case(){
    bool result = true;
    u32 itterate_num = 120;
    Uart uart_tx(0);
    if( uart_tx.open(Uart::RDWR|Uart::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
        result = false;
    } else {
        UartPinAssignment uart_pin_assignment;
        uart_pin_assignment->tx = mcu_pin(0,9);
        uart_pin_assignment->rx = mcu_pin(0,10);
        UartAttr uart_attr;//not used
        uart_tx.set_attr(UART_FLAG_SET_LINE_CODING|UART_FLAG_IS_PARITY_EVEN,115200,9,uart_pin_assignment);
        for (u32 i =0;i<itterate_num;i++){
            String message(messege_text);
            {
                Aio aio_t(messege_text, sizeof(messege_text)); //aio uses buf as it's data
                uart_tx.flush();    //flush before recved packet
                uart_tx.write(aio_t);
                while( !aio_t.is_done()){
                    Timer::wait_msec(5); //wait for the operation to complete
                }
            }
        }
        if( uart_tx.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
            result = false;
        }
    }

    return result;
}

bool UartTest::execute_class_stress_case(){
    bool result = true;
    u32 itterate_num = 120;
    const u32 MAX_DATA_LEN_IN_U32 = 64;
    Uart uart_tx(0);
    if( uart_tx.open(Uart::RDWR|Uart::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
        result = false;
    } else {
        UartPinAssignment uart_pin_assignment;
        uart_pin_assignment->tx = mcu_pin(0,9);
        uart_pin_assignment->rx = mcu_pin(0,10);

        UartAttr uart_attr;//not used
        for (u32 i =0;i<itterate_num;i++){
            char recv_buff[sizeof(messege_text)];
            u32 freq,flags,width;
            flags = UART_FLAG_SET_LINE_CODING;
            flags |= get_parity(i);
            flags |= get_flags_stop_bit(i);
            if(i%3){
                width = 9;
            }else{
                width = 8;
            }
            freq = get_freq(i%MAX_FREQ_TYPES);
            if(uart_tx.set_attr(flags,freq,width,uart_pin_assignment)!=0){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                result = false;
                break;
            }
            String message(messege_text);
            {
                memset(recv_buff,0,message.size());
                Aio aio_r(recv_buff, sizeof(messege_text)); //aio uses buf as it's data
                Aio aio_t(messege_text, sizeof(messege_text)); //aio uses buf as it's data
                uart_tx.flush();    //flush before recved packet
                uart_tx.write(aio_t);
                while( !aio_t.is_done()){
                    Timer::wait_msec(5); //wait for the operation to complete
                }
                uart_tx.read(aio_r);
                while( !aio_r.is_done()){
                    Timer::wait_msec(5); //wait for the operation to complete
                }
                if(memcmp(messege_text,recv_buff,sizeof(messege_text))){
                    print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
                    print_case_message("recv %s : %s", recv_buff,messege_text);
                    result = false;
                    break;
                }
            }
        }
        if(uart_tx.set_attr(UART_FLAG_SET_LINE_CODING,115200,8,uart_pin_assignment)!=0){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
            result = false;
        }
        const u32 max_data_len = sizeof(u32)*MAX_DATA_LEN_IN_U32;
        char recv_buff[max_data_len];
        for (u32 i =sizeof(u32);i<max_data_len;i+=sizeof(u32)){
            int data_len = i;
            Data send_data;
            send_data.alloc(data_len);
            memset(recv_buff,0,data_len);
            u8 rand_value =(rand()&0xff);
            rand_value = rand_value > 0?rand_value:rand_value+1;
            send_data.fill(rand_value);
            Sys::assign_zero_sum32(send_data.cdata(), data_len);
            Aio aio_r(recv_buff, data_len); //aio uses buf as it's data
            Aio aio_t(send_data.cdata(), data_len); //aio uses buf as it's data
            uart_tx.flush();    //flush before recved packet
            uart_tx.write(aio_t);
            while( !aio_t.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            uart_tx.read(aio_r);
            while( !aio_r.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            if (!Sys::verify_zero_sum32 (recv_buff, data_len)){
                print_case_message("Failed %s %d: %d", __FILE__, __LINE__, data_len);
                print_case_message("recv %s : %s", recv_buff,messege_text);
                result = false;
                break;
            }
        }
        //set unavailable pin
        uart_pin_assignment->tx = mcu_pin(3,5);
        uart_pin_assignment->rx = mcu_pin(3,6);
        if(uart_tx.set_attr(UART_FLAG_SET_LINE_CODING,115200,8,uart_pin_assignment)==0){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
            result = false;
        }
        if( uart_tx.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart_tx.port());
            result = false;
        }
    }
    return result;
}
#if USE_TWO_UARTS
static void * thread_rx(void * args){
    //ThreadTest * thread_test= (ThreadTest*)args;
    (void)args;
    result = true;
    Uart uart_rx(1);
    if( uart_rx.open(Uart::RDWR) < 0 ){
        result = false;
    } else {
        UartPinAssignment uart_pin_assignment;
        uart_pin_assignment->tx = mcu_pin(3,5);
        uart_pin_assignment->rx = mcu_pin(3,6);
        uart_rx.init();
        uart_rx.set_attr(UART_FLAG_SET_LINE_CODING,115200,8,uart_pin_assignment);
        while(1){
            char recv_buff[5];
            Timer::wait_milliseconds(100);
            if(uart_rx.read(recv_buff,5)==5){
                Timer::wait_milliseconds(2);
                break;
            }
            //break;
        }
        byte_recv = true;
        if( uart_rx.close() < 0 ){
            result = false;
        }
    }
    return &result;
}
#endif
bool UartTest::execute_uart_api_case(Uart & uart){
    bool result = true;
    String test_name;

    test_name.sprintf("Uart:%d", uart.port());

    open_case(test_name.str());

    if( uart.open(Uart::RDWR|Uart::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart.port());
        result = false;
    } else {
		  uart.initialize();
        Core core(0);
        core_info_t core_info;
        core.open();
        core.get_info(core_info);
        mcu_board_config_t mcu_board_config;
        core.get_mcu_board_config(mcu_board_config);
        if(mcu_board_config.debug_uart_port!=uart.port()){
            if(uart.put('z')){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, mcu_board_config.debug_uart_port);
                result = false;
            }

            if(uart.put('p')){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart.port());
                result = false;
            }
        }
        if( uart.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart.port());
            result = false;
        }
    }
    close_case(result);
    return result;
}
void UartTest::get_uart_count(){
    PeriphObject::port_t uart_count = 0;
    bool open_result = true;

    do {
        Uart uart(uart_count);

        if( uart.open() < 0 ){
            open_result = false;
        } else {
            uart_count++;
            uart.close();
        }

    } while( open_result );

    m_uart_count = uart_count;
}
u32 get_freq(u32 i){
    u32 freq = 115200;
    switch(i){
    case(0):
        freq = 1200;
        break;
    case(1):
        freq = 2400;
        break;
    case(2):
        freq = 4800;
        break;
    case(3):
        freq = 9600;
        break;
    case(5):
        freq = 14400;
        break;
    case(6):
        freq = 19200;
        break;
    case(7):
        freq = 28800;
        break;
    case(8):
        freq = 38400;
        break;
    case(9):
        freq = 56000;
        break;
    case(10):
        freq = 76800;
        break;
    case(11):
        freq = 115200;
        break;
    default:
        freq = 115200;
        break;
    }
    return freq;
}
u32 get_flags_stop_bit(u32 i){
    u32 flags;
    flags = 0;
    switch (i%4){
        case(0):
        flags |= UART_FLAG_IS_STOP1;
        break;
        case(1):
        flags |= UART_FLAG_IS_STOP2;
        break;
        case(2):
        flags |= UART_FLAG_IS_STOP0_5;
        break;
        case(3):
        flags |= UART_FLAG_IS_STOP1_5;
        break;
        default:
        flags |= UART_FLAG_IS_STOP1;
    }
    return flags;
}
u32 get_parity(u32 i){
    u32 flags;
    flags = 0;
    switch (i%3){
        case(0):
        flags |= UART_FLAG_IS_PARITY_NONE;
        break;
        case(1):
        flags |= UART_FLAG_IS_PARITY_ODD;
        break;
        case(2):
        flags |= UART_FLAG_IS_PARITY_EVEN;
        break;
        default:
        flags |= UART_FLAG_IS_PARITY_NONE;
    }
    return flags;
}
