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
        uart_tx.set_attr(UART_FLAG_SET_LINE_CODING,115200,8,uart_pin_assignment);
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
            uart_tx.read(aio_r);    //flush before recved packet
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
            print_case_message("end");//witgout this line next section will't work
        }
        {
            message.append("12");
            char recv_buff[message.size()];
            Aio aio_r(recv_buff, sizeof(messege_text)); //aio uses buf as it's data
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
/*            message.append("last");
            char recv_buff[message.size()];
            Aio aio_r(recv_buff, sizeof(messege_text)); //aio uses buf as it's data
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
            }*/
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

    return result;
}

bool UartTest::execute_class_stress_case(){
    bool result = true;
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
        uart.init();
        Core core(0);
        mcu_board_config_t mcu_board_config;
        core.get_mcu_board_config(mcu_board_config);
        if(mcu_board_config.debug_uart_port!=uart.port()){
            if(uart.put('z')){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, uart.port());
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


