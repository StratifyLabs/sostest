#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/hal/Periph.hpp>
#include <sapi/hal/Emc.hpp>
#include "EmcTest.hpp"


EmcTest::EmcTest() : Test("hal::EMC"){

}

bool EmcTest::execute_class_api_case(){
    bool result = true;
    Device qspi0;
    if(qspi0.open("/dev/emc0", Device::RDWR)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }else{
        qspi0.close();                         //close the SPI (power it off)
    }
    Emc_psram psram(0);
    u32 o_flags = Emc_psram::FLAG_ENABLE|Emc_psram::FLAG_IS_PSRAM_BANK1;
    if(psram.init(o_flags,SRAM_SIZE,SRAM_BANK_ADDR,BUS_WIDTH)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }else{
        Array <u8,MAX_TEST_BUFFER_SIZE>write_array;
        Array <u8,MAX_TEST_BUFFER_SIZE>read_array;
        write_array.fill(12);
        psram.write(0,write_array.data(),write_array.size());
        psram.read(0,read_array.data(),read_array.size());
        for (u32 i=0;i<read_array.size();i++) {
            if (read_array.at(i)!=12){
                result = 0;
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                break;
            }
        }
        print_case_message("read data test %u",read_array.at(5));
        psram.close();
    }
    EmcAttributes emc_attr(EMC_FLAG_ENABLE,16,18,2,1);
    const emc_pin_assignment_t * pin_assignment;
    mcu_pin_t pins[18];
    mcu_pin_t pin = {3,4};    emc_attr.set_oe(pin);
    pin = {3,5};    emc_attr.set_we(pin);
    pins[0] = {3,7};    emc_attr.set_ncs(pins);
    pins[0] = {4,0};
    pins[1] = {4,1};    emc_attr.set_bl(pins);
    pins[0] = {3,14};
    pins[1] = {3,15};
    pins[2] = {3,0};
    pins[3] = {3,1};
    pins[4] = {4,7};
    pins[5] = {4,8};
    pins[6] = {4,9};
    pins[7] = {4,10};
    pins[8] = {4,11};
    pins[9] = {4,12};
    pins[10] = {4,13};
    pins[11] = {4,14};
    pins[12] = {4,15};
    pins[13] = {3,8};
    pins[14] = {3,9};
    pins[15] = {3,10};    emc_attr.set_data(pins);
    pins[0] = {5,0};
    pins[1] = {5,1};
    pins[2] = {5,2};
    pins[3] = {5,3};
    pins[4] = {5,4};
    pins[5] = {5,5};
    pins[6] = {5,12};
    pins[7] = {5,13};
    pins[8] = {5,14};
    pins[9] = {5,15};
    pins[10] = {6,0};
    pins[11] = {6,1};
    pins[12] = {6,2};
    pins[13] = {6,3};
    pins[14] = {6,4};
    pins[15] = {6,5};
    pins[16] = {3,11};
    pins[17] = {3,12};    emc_attr.set_address(pins);
    pin_assignment = emc_attr.pin_assignment();
    o_flags = Emc_psram::FLAG_ENABLE|Emc_psram::FLAG_IS_PSRAM_BANK1;
    if(psram.init(o_flags,SRAM_SIZE,SRAM_BANK_ADDR,BUS_WIDTH,pin_assignment)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }else{
        Array <u8,MAX_TEST_BUFFER_SIZE>write_array;
        Array <u8,MAX_TEST_BUFFER_SIZE>read_array;
        write_array.fill(12);
        print_case_message("write %u",write_array.size());
        psram.write(0,write_array.data(),write_array.size());
        psram.read(0,read_array.data(),read_array.size());
        for (u32 i=0;i<read_array.size();i++) {
            if (read_array.at(i)!=12){
                result = 0;
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                break;
            }
        }
        print_case_message("read data test %u",read_array.at(5));

        psram.close();
    }


    return result;
}


bool EmcTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool EmcTest::execute_class_stress_case(){
    bool result = true;
    Device qspi0;
    if(qspi0.open("/dev/emc0", Device::RDWR)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = 0;
    }else{
        qspi0.close();                         //close the SPI (power it off)
    }
    Emc_psram psram(0);
    u32 o_flags = Emc_psram::FLAG_ENABLE|Emc_psram::FLAG_IS_PSRAM_BANK1;
    if(psram.init(o_flags,SRAM_SIZE,SRAM_BANK_ADDR,BUS_WIDTH)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }else{
        Array <u8,MAX_TEST_BUFFER_SIZE>write_array;
        Array <u8,MAX_TEST_BUFFER_SIZE>read_array;
        write_array.fill(12);
        psram.write(0,write_array.data(),write_array.size());
        psram.read(0,read_array.data(),read_array.size());
        for (u32 i=0;i<read_array.size();i++) {
            if (read_array.at(i)!=12){
                result = 0;
                print_case_message("data not equal %u",read_array.at(i));
            }
        }
        u16 buffer_size;
        for (u32 l=0;l<5;l++){
            u32 j=0;
            u32 i=0;
            buffer_size = rand() & 0x1ff;
            buffer_size = (buffer_size < 2)?2:buffer_size;
            printf("buffer size %u",buffer_size);
            for (i=0;i<((u32)SRAM_SIZE-buffer_size);i+=buffer_size){
                j=0;
                write_array.fill((u8)i);
                psram.write(i,write_array.data(),buffer_size);
            }
            for (i=0;i<((u32)SRAM_SIZE-buffer_size);i+=buffer_size){
                psram.read(i,read_array.data(),buffer_size);
                for (j=0;j<buffer_size;j++){
                    if (read_array.at(j)!=(u8)i){
                        result = false;
                        printf("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                        break;
                    }
                }
                if(j<buffer_size){
                    result = false;
                    printf("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                    break;
                }
            }
            printf("fmc_psram0 tested - %lu ram space from - %lu : %lu de %u\n", i,((u32)SRAM_SIZE-buffer_size),l,buffer_size);
        }
        psram.close();
    }
    return result;
}

