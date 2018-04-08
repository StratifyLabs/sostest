#include <sapi/var.hpp>
#include "TokenTest.hpp"

TokenTest::TokenTest(): Test("var::Token"){

}




bool TokenTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool TokenTest::execute_class_stress_case(){
    bool result = true;

    return result;
}

bool TokenTest::execute_class_api_case(){
    bool result = true;
    String s1 = "Uno,dos,tres, quatro or  cinko";
    Token one(s1.c_str(),",. ",";",0);
    if(one.size() != 6){
        print_case_message("token", "token calculate failed");
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
    Token two(s_common.c_str(),",; ","(",0);
    for(u32 i =0;i<48;i++){
        if (strncmp(two.at(i),s[i%8],s[i%8].len())){
            result = false;
        }
    }

    return result;
}

