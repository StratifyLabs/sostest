#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include "I2STest.hpp"
I2STest::I2STest() : Test("hal::I2S"){

}


bool I2STest::execute_class_api_case(){
    bool result = true;

    get_i2s_count();

    print_case_message("Board has %d I2S", m_i2s_count);

    for(PeriphObject::port_t count = 0; count < m_i2s_count; count++){
        I2S i2s(count);
        if( execute_i2s_api_case(i2s) == false ){
            result = false;
        }
    }

    I2S i2s_tx(1);
    if( i2s_tx.open(I2S::RDWR|I2S::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2s_tx.port());
        result = false;
    } else {
        I2SPinAssignment i2s_pin_assignment;
        i2s_pin_assignment->sck = mcu_pin(2,2);    //pc2 i2s2
        i2s_pin_assignment->mck = mcu_pin(2,3);    //pc3 i2s2
        i2s_tx.set_attr(SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,1000000,8,i2s_pin_assignment);
		  i2s_tx.initialize();
            //char text[] = "hello_two";
        char send_buff[] = "i2s_test";
        char recv_buff[sizeof(send_buff)];
        if(memcmp(send_buff,recv_buff,sizeof(send_buff))){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2s_tx.port());
            result = false;
        }
        Timer::wait_milliseconds(100);
    }
    if( i2s_tx.close() < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2s_tx.port());
        result = false;
    }
    return result;
}


bool I2STest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool I2STest::execute_class_stress_case(){
    bool result = true;
    return result;
}

bool I2STest::execute_i2s_api_case(I2S & i2s){
    bool result = true;
    String test_name;

    test_name.sprintf("I2S:%d", i2s.port());

    open_case(test_name.str());

    if( i2s.open(I2S::RDWR) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2s.port());
        result = false;
    } else {

		  i2s.initialize();
        if( i2s.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2s.port());
            result = false;
        }
    }

    close_case(result);

    return result;
}
void I2STest::get_i2s_count(){
    PeriphObject::port_t i2s_count = 0;
    bool open_result = true;

    do {
        I2S i2s(i2s_count);

        if( i2s.open() < 0 ){
            open_result = false;
        } else {
            i2s_count++;
            i2s.close();
        }

    } while( open_result );

    m_i2s_count = i2s_count;
}


