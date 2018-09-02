#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include "I2CTest.hpp"
I2CTest::I2CTest() : Test("hal::I2C"){

}


bool I2CTest::execute_class_api_case(){
    bool result = true;

    get_i2c_count();

    print_case_message("Board has %d I2C", m_i2c_count);
    I2CAttr i2c_attr;
    i2c_attr.set_slave_addr(2);
    mcu_pin_t i2c2_scl;
    mcu_pin_t i2c2_sda;
    mcu_pin_t i2c3_scl;
    mcu_pin_t i2c3_sda;
    mcu_pin_t temp_pin;
    u8 slave_address_8 = 0xee;
    u16 slave_address_16 = 0xeeee;
    i2c2_scl.port = 1;
    i2c2_scl.pin = 10;
    i2c2_sda.port = 1;
    i2c2_sda.pin = 11;
    i2c3_scl.port = 0;
    i2c3_scl.pin = 8;
    i2c3_sda.port = 2;
    i2c3_sda.pin = 9;
    i2c_attr.set_scl(i2c2_scl);
    temp_pin = i2c_attr.scl();
    if(temp_pin.port!=i2c2_scl.port || temp_pin.pin!=i2c2_scl.pin){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    i2c_attr.set_sda(i2c2_sda);
    temp_pin = i2c_attr.sda();
    if(temp_pin.port!=i2c2_sda.port || temp_pin.pin!=i2c2_sda.pin){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    i2c_attr.set_slave_addr(slave_address_8);
    if(i2c_attr.slave_addr()!=slave_address_8){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    i2c_attr.set_slave_addr16(slave_address_16);
    if(i2c_attr.slave_addr16() !=slave_address_16){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }

    for(I2C::port_t count = 0; count < m_i2c_count; count++){
        I2C i2c(count);
        if( execute_i2c_api_case(i2c) == false ){
            result = false;
        }
    }

    I2C i2c_master(1);
    I2C i2c_slave(2);
    if( i2c_master.open(I2C::RDWR|I2C::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
        result = false;
    } else {
        if( i2c_slave.open(I2C::RDWR|I2C::NONBLOCK) < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
            result = false;
        } else {
            i2c_attr.set_scl(i2c2_scl);
            i2c_attr.set_sda(i2c2_sda);
            i2c_attr.set_slave_addr(slave_address_8);
            i2c_attr.set_flags(I2C_FLAG_SET_MASTER);
            i2c_attr.set_freq(100000);

            i2c_master.set_attr(i2c_attr);

            if( i2c_slave.close() < 0 ){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
                result = false;
            }
        }
        if( i2c_master.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
            result = false;
        }

    }
/*    if( i2c_master.open(I2C::RDWR|I2C::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
        result = false;
    } else {
        i2c_slave.open(I2C::RDWR|I2C::NONBLOCK);

        i2c_attr.set_scl(i2c2_scl);
        i2c_attr.set_sda(i2c2_sda);
        i2c_attr.set_slave_addr(slave_address_8);
        i2c_attr.set_flags(I2C_FLAG_SET_MASTER);
        i2c_attr.set_freq(100000);

        i2c_master.set_attr(i2c_attr);
        i2c_attr.set_flags(I2C_FLAG_SET_SLAVE);
        i2c_attr.set_freq(100000);
        i2c_attr.set_scl(i2c3_scl);
        i2c_attr.set_sda(i2c3_sda);
        i2c_attr.set_slave_addr(slave_address_8);
        i2c_slave.set_attr(i2c_attr);
            //char text[] = "hello_two";
        char messege_text[] = "i2c_test";
        char recv_buff[sizeof(messege_text)];
        while(1){
            i2c_master.write(messege_text,2);
            Timer::wait_msec(50); //wait for the operation to complete
        }
        Aio aio_r(recv_buff, sizeof(messege_text)); //aio uses buf as it's data
        Aio aio_t(messege_text, sizeof(messege_text)); //aio uses buf as it's data
        i2c_slave.read(aio_r);
        i2c_master.prepare(slave_address_8, I2C::FLAG_PREPARE_DATA);
        i2c_master.write(aio_t);

        while( !aio_r.is_done()){
            Timer::wait_msec(5); //wait for the operation to complete
        }
        while( !aio_t.is_done()){
            Timer::wait_msec(5); //wait for the operation to complete
        }
        if(memcmp(messege_text,recv_buff,sizeof(messege_text))){
            print_case_message("Failed %s %d:", __FILE__, __LINE__);
            print_case_message("recv %s ", recv_buff);
            result = false;
        }
        Timer::wait_milliseconds(100);
        if( i2c_master.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
            result = false;
        }
        if( i2c_slave.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
            result = false;
        }

    }*/
    return result;
}


bool I2CTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool I2CTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

bool I2CTest::execute_i2c_api_case(I2C & i2c){
    bool result = true;
    String test_name;

    test_name.sprintf("I2C:%d", i2c.port());

    open_case(test_name.str());

    if( i2c.open(I2C::RDWR) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c.port());
        result = false;
    } else {

        i2c.init();
        if( i2c.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c.port());
            result = false;
        }
    }

    close_case(result);

    return result;
}
void I2CTest::get_i2c_count(){
    PeriphObject::port_t i2c_count = 0;
    bool open_result = true;

    do {
        I2C i2c(i2c_count);

        if( i2c.open() < 0 ){
            open_result = false;
        } else {
            i2c_count++;
            i2c.close();
        }

    } while( open_result );

    m_i2c_count = i2c_count;
}


