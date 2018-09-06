#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include "SpiTest.hpp"
static u32 get_freq(u32 i);
static u32 get_flags_stop_bit(u32 i);
static u32 get_flags_parity(u32 i);

SpiTest::SpiTest() : Test("hal::Spi"){

}

bool SpiTest::execute_class_api_case(){
    bool result = true;
    get_spi_count();
    print_case_message("Board has %d Spi", m_spi_count);
    for(PeriphObject::port_t count = 0; count < m_spi_count; count++){
        Spi spi(count);
        if( execute_spi_api_case(spi) == false ){
            result = false;
        }
    }
    Spi spi_hal(1);
    if( spi_hal.open(Spi::RDWR|Spi::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
        result = false;
    } else {
        SpiPinAssignment spi_pin_assignment;
        spi_pin_assignment->miso = mcu_pin(2,2);    //pc2 spi2
        spi_pin_assignment->mosi = mcu_pin(2,3);    //pc3 spi2
        spi_pin_assignment->sck = mcu_pin(1,10);    //pb10 spi2
        spi_pin_assignment->cs = mcu_pin(1,12);    //pb12
        if(spi_hal.set_attr(SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,1000000,8,spi_pin_assignment)!=0){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
            result = false;
        }
            //char text[] = "hello_two";
        {
            char send_buff[] = "spi_test";
            char recv_buff[sizeof(send_buff)];
            if(spi_hal.transfer(send_buff,recv_buff,sizeof(send_buff))!=sizeof(send_buff)){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
                result = false;
            }
            if(memcmp(send_buff,recv_buff,sizeof(send_buff))){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
                result = false;
            }
        }
        {
            const u32 data_len = 4*4;
            char recv_buff[data_len];
            Data send_data;
            send_data.alloc(data_len);
            send_data.set_size(data_len);
            send_data.fill(12);
            //void Sys::assign_zero_sum32(void * data, int size);
            Sys::assign_zero_sum32(send_data.cdata(), data_len);
            int recv_data_len = spi_hal.transfer(send_data.cdata(),recv_buff,data_len);
            if(recv_data_len!=data_len){
                print_case_message("Failed %s %d: len:%d", __FILE__, __LINE__, recv_data_len);
                result = false;
            }
            if (!Sys::verify_zero_sum32 (recv_buff, data_len)){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
                result = false;
            }
            send_data.fill(11);
            Sys::assign_zero_sum32(send_data.cdata(), data_len);
            Aio aio_r(recv_buff, data_len); //aio uses buf as it's data
            Aio aio_t(send_data.cdata(), data_len); //aio uses buf as it's data
            spi_hal.read(aio_r);    //flush before recved packet
            spi_hal.write(aio_t);
            while( !aio_t.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            while( !aio_r.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            if (!Sys::verify_zero_sum32 (recv_buff, data_len)){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
                result = false;
            }
        }
        if( spi_hal.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
            result = false;
        }
    }
    return result;
}


bool SpiTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool SpiTest::execute_class_stress_case(){
    bool result = true;
    u32 itterate_num = 100;
    Spi spi_hal(1);
    SpiPinAssignment spi_pin_assignment;
    spi_pin_assignment->miso = mcu_pin(2,2);    //pc2 spi2
    spi_pin_assignment->mosi = mcu_pin(2,3);    //pc3 spi2
    spi_pin_assignment->sck = mcu_pin(1,10);    //pb10 spi2
    spi_pin_assignment->cs = mcu_pin(1,12);    //pb12

    if( spi_hal.open(Spi::RDWR|Spi::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
        result = false;
    } else {
        for(u32 i =0;i<itterate_num;i++){
            u32 freq = get_freq(i);
            u32 width;
            if((i%3)==2){
                width = 32;
            }else if((i%3)==1){
                width = 16;
            }else {
                width = 8;
            }
            if(spi_hal.set_attr(SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,freq,width,spi_pin_assignment)!=0){
                print_case_message("Failed %s %d: freq:%d,width :%d", __FILE__, __LINE__, freq,width);
                result = false;
                break;
            }
        }
        if(spi_hal.set_attr(SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,1000000,8,spi_pin_assignment)!=0){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
            result = false;
        }

        for(u32 i=0;i<itterate_num ;i++){
            const u32 data_len = 4*2;
            char recv_buff[data_len];
            Data send_data;
            send_data.alloc(data_len);
            send_data.set_size(data_len);
            u8 value = rand()&0xff;
            value = value == 0?0xee:value;
            send_data.fill(value);
            //void Sys::assign_zero_sum32(void * data, int size);
            Sys::assign_zero_sum32(send_data.cdata(), data_len);
            int recv_data_len = spi_hal.transfer(send_data.cdata(),recv_buff,data_len);
            if(recv_data_len!=data_len){
                print_case_message("Failed %s %d:%d", __FILE__, __LINE__, i);
                result = false;
                break;
            }
            if (!Sys::verify_zero_sum32 (recv_buff, data_len)){
                print_case_message("Failed %s %d:%d", __FILE__, __LINE__, i);
                result = false;
                break;
            }
            value = rand()&0xff;
            value = value == 0?0xee:value;
            send_data.fill(value);
            Sys::assign_zero_sum32(send_data.cdata(), data_len);
            Aio aio_r(recv_buff, data_len); //aio uses buf as it's data
            Aio aio_t(send_data.cdata(), data_len); //aio uses buf as it's data
            spi_hal.read(aio_r);    //flush before recved packet
            spi_hal.write(aio_t);
            while( !aio_t.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            while( !aio_r.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            if (!Sys::verify_zero_sum32 (recv_buff, data_len)){
                print_case_message("Failed %s %d:%d", __FILE__, __LINE__, i);
                result = false;
                break;
            }
        }
        if( spi_hal.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_hal.port());
            result = false;
        }
    }
    return result;
}

bool SpiTest::execute_spi_api_case(Spi & spi){
    bool result = true;
    String test_name;

    test_name.sprintf("Spi:%d", spi.port());

    open_case(test_name.str());

    if( spi.open(Spi::RDWR) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi.port());
        result = false;
    } else {

        spi.init();
        if( spi.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi.port());
            result = false;
        }
    }

    close_case(result);

    return result;
}
void SpiTest::get_spi_count(){
    PeriphObject::port_t spi_count = 0;
    bool open_result = true;

    do {
        Spi spi(spi_count);

        if( spi.open() < 0 ){
            open_result = false;
        } else {
            spi_count++;
            spi.close();
        }

    } while( open_result );

    m_spi_count = spi_count;
}

u32 get_freq(u32 i){
    const u32 MIN_FREQ = 100000;
    const u32 MAX_FREQ = 1000000;
    const u32 STEP_FREQ = (MAX_FREQ - MIN_FREQ)/100;
    u32 freq;
    freq = (i%100)* STEP_FREQ + MIN_FREQ;
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
u32 get_flags_parity(u32 i){
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

