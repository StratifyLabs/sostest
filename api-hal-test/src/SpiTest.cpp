#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include "SpiTest.hpp"
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

    Spi spi_tx(1);
    if( spi_tx.open(Spi::RDWR|Spi::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_tx.port());
        result = false;
    } else {
        SpiPinAssignment spi_pin_assignment;
        spi_pin_assignment->miso = mcu_pin(2,2);    //pc2 spi2
        spi_pin_assignment->mosi = mcu_pin(2,3);    //pc3 spi2
        spi_pin_assignment->sck = mcu_pin(1,10);    //pb10 spi2
        spi_pin_assignment->cs = mcu_pin(1,12);    //pb12
        spi_tx.set_attr(SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,1000000,8,spi_pin_assignment);
        spi_tx.init();
            //char text[] = "hello_two";
        char send_buff[] = "spi_test";
        char recv_buff[sizeof(send_buff)];
        if(spi_tx.transfer(send_buff,recv_buff,sizeof(send_buff))!=sizeof(send_buff)){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_tx.port());
            result = false;
        }
        if(memcmp(send_buff,recv_buff,sizeof(send_buff))){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_tx.port());
            result = false;
        }
        Timer::wait_milliseconds(100);
    }
    if( spi_tx.close() < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, spi_tx.port());
        result = false;
    }
    return result;
}


bool SpiTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool SpiTest::execute_class_stress_case(){
    bool result = true;
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


