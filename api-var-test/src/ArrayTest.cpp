#include <sapi/var.hpp>
#include "ArrayTest.hpp"

ArrayTest::ArrayTest(): Test("var::Array"){

}



/*! \details test "performance" a var::Array
 *
 * @return false if some test failed
 */

bool ArrayTest::execute_class_performance_case(){
    bool result = true;
    Array <u8,1000>u8_array;
    Array <u32,500>u32_array;
    Array <double,350>double_array;
    u8 u8_value;
    u32 u32_value;
    double double_value;
    for (u16 j = 0;j< 255;j++){
        u8_value = j;
        u8_array.fill(u8_value);
        for (u32 i=0;i<u8_array.size();i++){
            if(u8_array.at(i)!=u8_value){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
        u32_value = j*1737;
        u32_array.fill(u32_value);
        for (u32 i=0;i<u32_array.size();i++){
            if(u32_array.at(i)!=u32_value){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
        double_value = j&0x01?3.1456*(double)j:-3.1456*(double)j;
        double_array.fill(double_value);
        for (u32 i=0;i<double_array.size();i++){
            if(double_array.at(i)!=double_value){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }
    return result;
}
/*! \details test "stress" a var::Array
 *
 * @return false if some test failed
 */

bool ArrayTest::execute_class_stress_case(){
    bool result = true;

    return result;
}
/*! \details test "api" a var::Array
 *  fill,at,front,back
 * @return false if some test failed
 */
bool ArrayTest::execute_class_api_case(){
    bool result = true;

    Array <u8,1000>u8_array;
    Array <u32,500>u32_array;
    Array <double,350>double_array;
    Array <String,6>string_array;
    String s1("fill");
    string_array.fill(s1);
    String s2,s3;
    s2 = string_array.at(1);
    s3 = string_array.at(2);
    if(s3.compare(s2)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    for (u8 i=0;i<6;i++){
        String temp_string;
        temp_string = string_array.at(i);
        if(temp_string.compare(s1)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    u8 u8_value;
    u32 u32_value;
    double double_value;
    u8_value = 255;
    u32_value = 0xffffffff;
    double_value = -3.1e+48;
    u8_array.fill(u8_value);
    u32_array.fill(u32_value);
    double_array.fill(double_value);
    if((u8_array.at(500)!=u8_value)||(u8_array.back()!=u8_value)||(
                u8_array.front()!=u8_value)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((u32_array.at(250)!=
        u32_value)||(u32_array.back()!=u32_value)||(
                u32_array.front()!=u32_value)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((double_array.at(175)!=double_value)||(double_array.back()!=double_value)||(
                double_array.front()!=double_value)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    u8_value = 0;
    u32_value = 0;
    double_value = 0.0;
    u8_array.fill(u8_value);
    u32_array.fill(u32_value);
    double_array.fill(double_value);
    if((u8_array.at(500)!=u8_value)||(u8_array.back()!=u8_value)||(
                u8_array.front()!=u8_value)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((u32_array.at(250)!=u32_value)||(u32_array.back()!=u32_value)||(
                u32_array.front()!=u32_value)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((double_array.at(175)!=double_value)||(double_array.back()!=double_value)||(
                double_array.front()!=double_value)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    return result;
}

