#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/hal/Periph.hpp>
#include <sapi/hal/QSPI.hpp>
#include "sos/dev/qspi.h"
#include "QSPITest.hpp"

QSPITest::QSPITest() : Test("hal::QSPI"){

}


bool QSPITest::execute_class_api_case(){
    #define TEST_BUFFER_SIZE         256
    bool result = true;
    Device qspi0;
    if(qspi0.open("/dev/qspi0", Device::RDWR)<0){
        print_case_message("failed open qspi0");
    }else{
        print_case_message("succes open qspi0");
        qspi0.close();                         //close the SPI (power it off)
    }
    QSPI_FLASH qspi(0);
    u32 o_flags = QSPI_FLAG_SET_MASTER;
    u8 width = 26;    //2^26 - 64 mbytes /*set size flash */
    u32 freq = 1;/*clock prescaller 1 -> (mcu clock / 2)*/
    u32 read_instruction = QPI_READ_4_BYTE_ADDR_CMD;
    u32 mem_mapped_read_instruction = QUAD_OUT_FAST_READ_CMD;
    u32 write_instruction = QPI_PAGE_PROG_4_BYTE_ADDR_CMD;
    u32 dummy_cycle = QSPI_DUMMY_CYCLES_READ_QUAD_IO;
    if(qspi.init(o_flags,freq,width,read_instruction,mem_mapped_read_instruction,write_instruction,dummy_cycle)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }else{
        qspi.set_commands(READ_STATUS_REG_CMD,WRITE_ENABLE_CMD,QSPI_SR_WREN,QSPI_SR_WIP);
        qspi.set_page_size(QSPI_PAGE_SIZE);
        if(qspi.set_qspi_mode(ENTER_QUAD_CMD, QSPI_SR_QUADEN) < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }else{
            u8 status;
            status = qspi.read_status_reg();
            print_case_message("succes setmode QSPI %u",status );
            qspi.set_to_four_bytes_address(ENTER_4_BYTE_ADDR_MODE_CMD);
            u8 dummy_and_strength,dummy_and_strength_mask;
            dummy_and_strength = (QSPI_DUMMY_CYCLES_READ_QUAD << 6)|QSPI_CR_ODS_15;
            dummy_and_strength_mask = QSPI_CR_NB_DUMMY | QSPI_CR_ODS;
            if(qspi.set_dummy_cycles_and_strength(dummy_and_strength,dummy_and_strength_mask,READ_CFG_REG_CMD,WRITE_STATUS_CFG_REG_CMD)<0){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
            }else{
                print_case_message("dummy and stregth set");
            }
            qspi.erase_block(0,SUBSECTOR_ERASE_4_BYTE_ADDR_CMD);
            u8 test_buffer_read[TEST_BUFFER_SIZE];
            u8 test_buffer_write[TEST_BUFFER_SIZE];
            for (u16 i = 0;i<TEST_BUFFER_SIZE;i++){
                test_buffer_write[i] = (u8)i;
            }
            qspi.write_flash(0,test_buffer_write,TEST_BUFFER_SIZE);
            u32 len = qspi.read_flash(0,test_buffer_read,TEST_BUFFER_SIZE);
            if(len!=TEST_BUFFER_SIZE){
                print_case_message("size read failed %u ",len);
                result = false;
            }
            u16 i=0;
            for (i=0;i<TEST_BUFFER_SIZE;i++){
                if (test_buffer_read[i]!=test_buffer_write[i]){
                    break;
                }
            }
            if(i>=TEST_BUFFER_SIZE){
                print_case_message("passed equal ");
            }else{
                result = false;
                print_case_message("Failed %s:%d:%u", __PRETTY_FUNCTION__, __LINE__,i);
            }
        }
        qspi.close();
    }
    QSPIAttributes qspi_attr;
    const qspi_pin_assignment_t * pin_assignment;
    mcu_pin_t pin = {1,6};    qspi_attr.set_cs(pin);
    pin = {1,2};    qspi_attr.set_sck(pin);
    pin = {2,9};    qspi_attr.set_data0(pin);
    pin = {2,10};    qspi_attr.set_data1(pin);
    pin = {3,13};    qspi_attr.set_data2(pin);
    pin = {4,2};    qspi_attr.set_data3(pin);
    pin_assignment = qspi_attr.pin_assignment();
    if(qspi.open()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }else{
        if(qspi.set_attr(o_flags,freq,width,read_instruction,mem_mapped_read_instruction,write_instruction,dummy_cycle,pin_assignment)<0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }else{
            print_case_message("opened with pin assignment");
        }
    }
    qspi.close();
    return result;
}


bool QSPITest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool QSPITest::execute_class_stress_case(){
    bool result = true;
    int i ,j;
    QSPI_FLASH qspi(0);
    u8 test_buffer_read[TEST_BUFFER_SIZE];
    u8 test_buffer_write[TEST_BUFFER_SIZE];
    u32 o_flags = QSPI_FLAG_SET_MASTER;
    u8 width = 26;    //2^26 - 64 mbytes /*set size flash */
    u32 freq = 1;/*clock prescaller 1 -> (mcu clock / 2)*/
    u32 read_instruction = QPI_READ_4_BYTE_ADDR_CMD;
    u32 mem_mapped_read_instruction = QUAD_OUT_FAST_READ_CMD;
    u32 write_instruction = QPI_PAGE_PROG_4_BYTE_ADDR_CMD;
    u32 dummy_cycle = QSPI_DUMMY_CYCLES_READ_QUAD_IO;
    if(qspi.init(o_flags,freq,width,read_instruction,mem_mapped_read_instruction,write_instruction,dummy_cycle)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }else{
        print_case_message("succes init QSPI");
        qspi.set_commands(READ_STATUS_REG_CMD,WRITE_ENABLE_CMD,QSPI_SR_WREN,QSPI_SR_WIP);
        qspi.set_page_size(QSPI_PAGE_SIZE);
        if(qspi.set_qspi_mode(ENTER_QUAD_CMD, QSPI_SR_QUADEN) < 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }else{
            u8 status;
            status = qspi.read_status_reg();
            print_case_message("succes setmode QSPI %u",status );
            qspi.set_to_four_bytes_address(ENTER_4_BYTE_ADDR_MODE_CMD);
            u8 dummy_and_strength,dummy_and_strength_mask;
            dummy_and_strength = (QSPI_DUMMY_CYCLES_READ_QUAD << 6)|QSPI_CR_ODS_15;
            dummy_and_strength_mask = QSPI_CR_NB_DUMMY | QSPI_CR_ODS;
            if(qspi.set_dummy_cycles_and_strength(dummy_and_strength,dummy_and_strength_mask,READ_CFG_REG_CMD,WRITE_STATUS_CFG_REG_CMD)<0){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
            }else{
                print_case_message("dummy and stregth set");
            }

            for (i=0;(i*TEST_BUFFER_SIZE)<(FLASH_SIZE/4-TEST_BUFFER_SIZE);i++){
                j=0;
                memset(&test_buffer_write,(u8)(i),TEST_BUFFER_SIZE);
                if(((i*TEST_BUFFER_SIZE + TEST_BUFFER_SIZE)/QSPI_SECTOR_SIZE != \
                        (i*TEST_BUFFER_SIZE)/QSPI_SECTOR_SIZE)){
                    qspi.erase_block((u32)((i*TEST_BUFFER_SIZE + TEST_BUFFER_SIZE)/QSPI_SECTOR_SIZE)*QSPI_SECTOR_SIZE,SUBSECTOR_ERASE_4_BYTE_ADDR_CMD);
                }
                if(((i*TEST_BUFFER_SIZE + TEST_BUFFER_SIZE)/(QSPI_SECTOR_SIZE*256) != \
                    (i*TEST_BUFFER_SIZE)/(QSPI_SECTOR_SIZE*256))||((i*TEST_BUFFER_SIZE % (QSPI_SECTOR_SIZE*256))==0)){
                    print_case_message("writed %lu\n",i*TEST_BUFFER_SIZE);
                }
                if(qspi.write_flash((u32)(i*TEST_BUFFER_SIZE),test_buffer_write,TEST_BUFFER_SIZE)!=TEST_BUFFER_SIZE){
                    result = false;
                    print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                }
            }
            print_case_message("last writed address %lu \n",i*TEST_BUFFER_SIZE);
            for (i=0;(i*TEST_BUFFER_SIZE)<(FLASH_SIZE/4-TEST_BUFFER_SIZE);i++){
                if(qspi.read_flash((u32)(i*TEST_BUFFER_SIZE),test_buffer_read,TEST_BUFFER_SIZE)!=TEST_BUFFER_SIZE){
                    print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                    result = false;
                }
                j=0;
                for (j=0;j<TEST_BUFFER_SIZE;j++){
                    if (test_buffer_read[j]!=(u8)(i)){
                        print_case_message("error verification additional %lu %u %lu \n",j,test_buffer_read[j],i);
                        print_case_message("error verification additional %u %u %u %u\n",test_buffer_read[j-1],test_buffer_read[j+1],
                                test_buffer_read[j+2],test_buffer_read[j+3]);
                        break;
                    }
                }
                if(((i*TEST_BUFFER_SIZE + TEST_BUFFER_SIZE)/(QSPI_SECTOR_SIZE*256) != \
                    (i*TEST_BUFFER_SIZE)/(QSPI_SECTOR_SIZE*256))||((i*TEST_BUFFER_SIZE % (QSPI_SECTOR_SIZE*256))==0)){
                    print_case_message("readed %lu\n",i*TEST_BUFFER_SIZE);
                }

                if(j<TEST_BUFFER_SIZE){
                    result = false;
                    print_case_message("Failed %s:%d:%u", __PRETTY_FUNCTION__, __LINE__,j);
                    break;
                }
            }
            print_case_message("qspi flash tested - %lu ram space from - %lu \n", i*TEST_BUFFER_SIZE,(FLASH_SIZE-TEST_BUFFER_SIZE));
        }
        qspi.close();
    }
    return result;
}

