#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include "I2CTest.hpp"
I2CTest::I2CTest() : Test("hal::I2C"){

}

static void * thread_rx(void * args);
static int result = 0;
static bool mess_recv = false;
static u8 thread_running = 1;
//pin for I2C i2c(0)
const u8 i2c1_sda_port = 1; //port b
const u8 i2c1_scl_port = 1; //port b
const u8 i2c1_sda_pin = 9; //port b
const u8 i2c1_scl_pin = 8; //port b
//pin for I2C i2c(1)
const u8 i2c2_sda_port = 1; //port b
const u8 i2c2_scl_port = 1; //port b
const u8 i2c2_sda_pin = 11; //port b
const u8 i2c2_scl_pin = 10; //port b
//pin for I2C i2c(2)
const u8 i2c3_sda_port = 2; //port b
const u8 i2c3_scl_port = 0; //port b
const u8 i2c3_sda_pin = 9; //port b
const u8 i2c3_scl_pin = 8; //port b

const u8 slave_address_8 = 0x30;
static u16 data = 0x1234;

bool I2CTest::execute_class_api_case(){
    bool result = true;
    get_i2c_count();
    print_case_message("Board has %d I2C", m_i2c_count);
    I2CAttr i2c_attr;
    i2c_attr.set_slave_addr(2);
    mcu_pin_t i2c1_scl;
    mcu_pin_t i2c1_sda;
    mcu_pin_t i2c2_scl;
    mcu_pin_t i2c2_sda;
    mcu_pin_t temp_pin;

    u16 slave_address_16 = 0x4c4c;
    i2c1_scl.port = i2c1_scl_port;
    i2c1_scl.pin = i2c1_scl_pin;
    i2c1_sda.port = i2c1_sda_port;
    i2c1_sda.pin = i2c1_sda_pin;
    i2c2_scl.port = i2c2_scl_port;
    i2c2_scl.pin = i2c2_scl_pin;
    i2c2_sda.port = i2c2_sda_port ;
    i2c2_sda.pin = i2c2_sda_pin;
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

    I2C i2c_master(0);
    I2C i2c_slave(1);
    if( i2c_master.open(I2C::RDWR|I2C::NONBLOCK) < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
        result = false;
    } else {
        if(i2c_slave.open(I2C::RDWR|I2C::NONBLOCK)){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
            result = false;
        }else{
            i2c_attr.set_scl(i2c1_scl);
            i2c_attr.set_sda(i2c1_sda);
            i2c_attr.set_flags(I2C_FLAG_SET_MASTER);
            i2c_attr.set_freq(100000);
            if(i2c_master.set_attr(i2c_attr)!=0){
                print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, i2c_master.port());
                result = false;
            }
            i2c_attr.set_flags(I2C_FLAG_SET_SLAVE);
            i2c_attr.set_freq(100000);
            i2c_attr.set_scl(i2c2_scl);
            i2c_attr.set_sda(i2c2_sda);
            i2c_attr.set_slave_addr(slave_address_8);
            if(i2c_slave.set_attr(i2c_attr)){
                print_case_message("Failed %s %d", __FILE__, __LINE__);
                result = false;
            }
            char recv_buff[sizeof(data)];
            Aio aio_r(recv_buff, sizeof(data)); //aio uses buf as it's data
            Aio aio_t(&data, sizeof(data)); //aio uses buf as it's data
            i2c_slave.read(aio_r);
            i2c_master.prepare(slave_address_8, I2C::FLAG_PREPARE_DATA);
            i2c_master.write(aio_t);
            while( !aio_r.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            while( !aio_t.is_done()){
                Timer::wait_msec(5); //wait for the operation to complete
            }
            if(memcmp(&data,recv_buff,sizeof(data))){
                print_case_message("Failed %s %d:", __FILE__, __LINE__);
                print_case_message("recv %s ", recv_buff);
                result = false;
            }
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
    return result;
}


bool I2CTest::execute_class_performance_case(){
    bool result = true;
    I2CPinAssignment pin_assignment;
    Thread uno_thread(1024);
    result = true;
    I2C i2c_slave(0);
        thread_running = 1;
        pin_assignment->scl = mcu_pin(i2c1_scl_port,i2c1_scl_pin);
        pin_assignment->sda = mcu_pin(i2c1_sda_port,i2c1_sda_pin);
        i2c_slave.init(I2C::FLAG_SET_MASTER, 100000, pin_assignment);
        i2c_slave.prepare(slave_address_8,I2C_FLAG_PREPARE_DATA); //prepare for data only transactions
        i2c_slave.write(&data, sizeof(data)); //write two bytes

        i2c_slave.close();


    return result;
}

bool I2CTest::execute_class_stress_case(){
    bool result = true;

    I2CPinAssignment pin_assignment;
    Thread uno_thread(1024);
    i2c_attr_t i2c_attr_s;
    mcu_pin_t i2c1_scl,i2c1_sda;
    result = true;
    u16 answer[] = {0xf1f1,0xf1f1,0xf1f1,0xf1f1};
    u16 write_data[] = {0x0001,0x0003,0x0007,0x000f};
    i2c1_scl.port = i2c1_scl_port;
    i2c1_scl.pin = i2c1_scl_pin;
    i2c1_sda.port = i2c1_sda_port ;
    i2c1_sda.pin = i2c1_sda_pin;

    i2c_attr_s.pin_assignment.scl = i2c1_scl;
    i2c_attr_s.pin_assignment.sda = i2c1_sda;
    i2c_attr_s.freq = 100000;
    i2c_attr_s.o_flags = I2C_FLAG_SET_SLAVE;
    i2c_attr_s.slave_addr[0].addr8[0] = slave_address_8;
    i2c_attr_s.data = &answer;
    i2c_attr_s.size = sizeof(answer);
    I2C i2c_master(1);
    I2C i2c_slave(0);
    if( i2c_slave.open() != 0 ){
        result = false;
    } else {
        if(i2c_slave.set_attr(i2c_attr_s)){
            result = false;
        }
        i2c_info_t i2c_info;
        i2c_slave.get_info(i2c_info);
        printf("slave freq - %lu,flag - %lu",i2c_info.freq,i2c_info.o_flags);

        thread_running = 1;
        pin_assignment->scl = mcu_pin(i2c2_scl_port,i2c2_scl_pin);
        pin_assignment->sda = mcu_pin(i2c2_sda_port,i2c2_sda_pin);
        u16 recv_data[4];
        u8 recv;
        int loc = 0;
        memset(recv_data,0,sizeof(answer));
        i2c_master.init(I2C::FLAG_SET_MASTER, 100000, pin_assignment);
        i2c_master.get_info(i2c_info);
        print_case_message("master freq - %lu,flag - %lu",i2c_info.freq,i2c_info.o_flags);
        //i2c_master.prepare(slave_address_8,I2C_FLAG_PREPARE_DATA); //prepare for data only transactions
//        i2c_master.write(&data, sizeof(data)); //write two bytes
       // i2c_master.read(&recv_data,sizeof(data)); //read two bytes

        i2c_master.prepare(slave_address_8,I2C::FLAG_PREPARE_PTR_DATA); //prepare for data only transactions
        //i2c_slave.prepare(slave_address_8,I2C::FLAG_PREPARE_PTR_DATA); //prepare for data only transactions
/*        i2c_master.read(loc, recv); //write two bytes
        i2c_master.read(loc, recv); //write two bytes
        i2c_master.read(loc, recv); //write two bytes
        i2c_master.read(loc, recv); //write two bytes*/
        //i2c_master.prepare(slave_address_8,I2C_FLAG_PREPARE_DATA); //prepare for data only transactions
        //i2c_master.write(loc,0x00);

        i2c_master.read(loc, recv); //write two bytes
        //loc++;
        //i2c_master.read(loc, recv); //write two bytes
        //i2c_master.read(recv_data,sizeof(answer)); //read two bytes
        print_case_message("data read %u %u %u %u %u",recv_data[0],recv_data[1],recv_data[2],recv_data[3],recv);
        print_case_message("data write %u %u %u %u",answer[0],answer[1],answer[2],answer[3]);
        i2c_master.close();
        i2c_slave.close();
    }
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

static void * thread_rx(void * args){
    //ThreadTest * thread_test= (ThreadTest*)args;
    (void)args;
    I2C * i2c_slave = (I2C*)args;
    i2c_info_t i2c_info;
    i2c_slave->get_info(i2c_info);
    printf("thread started freq - %lu,flag - %lu",i2c_info.freq,i2c_info.o_flags);
    while(thread_running){
        char recv_buff[5];

/*        if(i2c_slave->read(recv_buff,1)){
            mess_recv = true;
            printf("recv messege");
        }*/
    }
    printf("thread stoped");
    return &result;
}
