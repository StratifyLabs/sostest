
#include <sapi/sys.hpp>
#include <sapi/calc.hpp>

#include "ChecksumTest.hpp"

ChecksumTest::ChecksumTest() : Test("calc::Checksum"){

}

typedef struct MCU_PACK {
  u8 member_a;
  u32 member_b;
  u16 member_c;
  u8 checksum;
} data_struct_u8_t;

typedef struct MCU_PACK {
  u16 member_a;
  u16 member_b;
  u32 member_c;
  u32 checksum;
} data_struct_u32_t;
typedef struct MCU_PACK {
  u8 buffer[2566];
  u8 checksum;
} data_buffer_u8_t;

typedef struct MCU_PACK {
  u32 buffer[2021];
  u32 checksum;
} data_buffer_u32_t;


bool ChecksumTest::execute_class_api_case(){
    bool result = true;
    //u8
    {
        data_struct_u8_t data;
        //set member variables
        data.member_a = rand();
        data.member_b = rand();
        data.member_c = rand();
        data.checksum = Checksum::calc_zero_sum((u8*)&data, sizeof(data));
        if( Checksum::verify_zero_sum((u8*)&data, sizeof(data)) != true ){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //u32
    {
        data_struct_u32_t data;
        //set member variables
        data.member_a = rand();
        data.member_b = rand();
        data.member_c = rand();
        data.checksum = Checksum::calc_zero_sum((u32*)&data, sizeof(data));
        if( Checksum::verify_zero_sum((u32*)&data, sizeof(data)) != true){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //zero buff
    //u8_buff
    {
        data_buffer_u8_t data;
        //set member variables
        for (u16 j=0; j<(sizeof(data)-1);j++){
            data.buffer[j] = 0;
        }
        data.checksum = 0xff;
        data.checksum = Checksum::calc_zero_sum((u8*)&data, sizeof(data));
        if( data.checksum != 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //u32_buff
    {
        data_buffer_u32_t data;
        //set member variables
        for (u16 j=0; j<(sizeof(data)/4-1);j++){
            data.buffer[j] = 0;
        }
        data.checksum = 0xffffffff;
        data.checksum = Checksum::calc_zero_sum((u32*)&data, sizeof(data));
        if( data.checksum != 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //imposible value
    //zero size
    {
        data_struct_u8_t data;
        //set member variables
        data.member_a = rand();
        data.member_b = rand();
        data.member_c = rand();
        data.checksum = Checksum::calc_zero_sum((u8*)&data, 0);
        Checksum::verify_zero_sum((u8*)&data, 0);
    }
    {
        data_struct_u8_t data;
        //set member variables
        data.member_a = rand();
        data.member_b = rand();
        data.member_c = rand();
        data.checksum = Checksum::calc_zero_sum((u32*)&data, 0);
        Checksum::verify_zero_sum((u32*)&data, 0);
    }

    return result;
}



bool ChecksumTest::execute_class_performance_case(){
    bool result = true;
    //u8
    {
        data_struct_u8_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            data.member_a = rand();
            data.member_b = rand();
            data.member_c = rand();
            data.checksum = Checksum::calc_zero_sum((u8*)&data, sizeof(data));
            if( Checksum::verify_zero_sum((u8*)&data, sizeof(data)) != true ){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    {
        data_struct_u32_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            data.member_a = rand();
            data.member_b = rand();
            data.member_c = rand();
            data.checksum = Checksum::calc_zero_sum((u32*)&data, sizeof(data));
            if( Checksum::verify_zero_sum((u32*)&data, sizeof(data)) != true ){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    {
        data_buffer_u8_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            for (u16 j=0; j<(sizeof(data)-1);j++){
                data.buffer[j] = rand();
            }
            data.checksum = Checksum::calc_zero_sum((u8*)&data, sizeof(data));
            if( Checksum::verify_zero_sum((u8*)&data, sizeof(data)) != true ){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    {
        data_buffer_u32_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            for (u16 j=0; j<(sizeof(data)/4-1);j++){
                data.buffer[j] = rand();
            }
            data.checksum = Checksum::calc_zero_sum((u32*)&data, sizeof(data));
            if( Checksum::verify_zero_sum((u32*)&data, sizeof(data)) != true ){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    //fill 0xff value
    {
        data_buffer_u8_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            for (u16 j=0; j<(sizeof(data)-1);j++){
                data.buffer[j] = 0xff;
            }
            data.checksum = Checksum::calc_zero_sum((u8*)&data, sizeof(data));
            if( Checksum::verify_zero_sum((u8*)&data, sizeof(data)) != true ){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    //fill 0xffffffff value
    {
        data_buffer_u32_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            for (u16 j=0; j<(sizeof(data)/4-1);j++){
                data.buffer[j] = 0xffffffff;
            }
            data.checksum = Checksum::calc_zero_sum((u32*)&data, sizeof(data));
            if( Checksum::verify_zero_sum((u32*)&data, sizeof(data)) != true ){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    return result;
}



bool ChecksumTest::execute_class_stress_case(){
    bool result = true;
    u32 data_buffer[3];
    for (u16 i =1;i<(2000);i++){
        //set member variables
        u16 shift;
        shift = rand()&0xffff;
        data_buffer[0] = rand()<<shift;
        data_buffer[1] = Checksum::calc_zero_sum((u32*)&data_buffer, sizeof(data_buffer)-4);
        if( Checksum::calc_zero_sum((u32*)&data_buffer, sizeof(data_buffer)) != 0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
            break;
        }

    }
    //imposible value
    //fill 0xffffffff value
    {
        data_buffer_u32_t data;
        for (u16 i =0;i<1000;i++){
            //set member variables
            for (u16 j=0; j<(sizeof(data)/4-1);j++){
                data.buffer[j] = 0xffffffff;
            }
            data.checksum = Checksum::calc_zero_sum((u32*)&data, 0);
            Checksum::verify_zero_sum((u32*)&data, 0);
        }
    }

    return result;
}

