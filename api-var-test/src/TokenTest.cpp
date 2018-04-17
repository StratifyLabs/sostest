#include <sapi/var.hpp>
#include "TokenTest.hpp"

TokenTest::TokenTest(): Test("var::Token"){

}

bool TokenTest::execute_class_performance_case(){
    bool result = true;
    String s[8];
    for(u32 j=0;j<1000;j++){
        s[0] = "uno";
        s[1] = "dos";
        s[2] = "tres";
        s[3] = "quatro";
        s[4] = "cinko";
        s[5] = "seis";
        s[6] = "sieta";
        s[7] = "ocho";
        String s_common;
        //generate string
        for (u32 i =0;i<50;i++){
            s_common.append(s[i%8].c_str());
            //add character from "space" to "/" in ascii table
            char char_ascii = 0x20 | (i & 0x0f);
            s_common.append(char_ascii);
        }
        //parse string and verify
        char delim[0x0f+2] = "\0";
        for (u8 i=0;i<=0x0f;i++){
            delim[i] = 0x20 | i;
        }
        Token two(s_common.c_str(),delim,0);
        for(u32 i =0;i<50;i++){
            if (strncmp(two.at(i),s[i%8].c_str(),s[i%8].len())){
                print_case_message("%s",delim);
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                return result;
            }
        }
    }
    return result;
}

bool TokenTest::execute_class_stress_case(){
    bool result = true;

    return result;
}
/*! \details test "api" a var::Token
 *  constructor,at(element),
 *  @return false if some test failed
 */
bool TokenTest::execute_class_api_case(){
    bool result = true;
    String s1 = "Uno,dos,tres, quatro or  cinko";
    Token one(s1.c_str(),",. ",";",0);
    if(one.size() != 6){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    String s[8];
    s[0] = "uno";
    s[1] = "dos";
    s[2] = "tres";
    s[3] = "quatro";
    s[4] = "cinko";
    s[5] = "seis";
    s[6] = "sieta";
    s[7] = "ocho";
    String s_common;
    //generate string for token
    for (u32 i =0;i<48;i++){
        s_common.append(s[i%8].c_str());
        if(i<16){
            s_common.append(",");
        }else if(i<32){
            s_common.append(";");
        }else if(i<47){
            s_common.append(" ");
        }
    }
    //parse string for and verify with base
    Token two(s_common.c_str(),",; ","(",0);
    for(u32 i =0;i<48;i++){
        if (strncmp(two.at(i),s[i%8],s[i%8].len())){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    for(u8 j=0;j<=0x0f;j++){
        char char_ascii;
        s_common.assign(s[0].c_str());
        //generate string
        for (u32 i =0;i<100;i++){
            s_common.append(s[i%8].c_str());
            //add character from "space" to "/" in ascii table
            char_ascii = 0x20 | j;
            s_common.append(char_ascii);
        }
        //parse string and verify
        char delim[2] = "\0";
        delim[0] = 0x20 | j;
        Token two(s_common.c_str(),delim,0);
        for(u32 i =0;i<100;i++){
            if (strncmp(two.at(i),s[i%8].c_str(),s[i%8].len())){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
        }
    }

    //add parse with not zero
    return result;
}

