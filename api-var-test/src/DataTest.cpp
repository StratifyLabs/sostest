
#include <cstdlib>
#include <sapi/sys.hpp>

#include "DataTest.hpp"

DataTest::DataTest() : Test("var::Data"){

}


bool DataTest::execute_class_api_case(){
    bool result = true;

    if( execute_fill() == false ){
        set_case_message("why:0", "execute_fill()");
        result = false;
    }

    if( execute_alloc() == false ){
        set_case_message("why:0", "execute_fill()");
        result = false;
    }

    return result;
}

bool DataTest::execute_fill(){
    bool result = true;
    Data data(128);
    char buffer[128];


    memset(buffer, 0, 128);
    data.fill(0);
    if( memcmp(data.data_const(), buffer, 128) != 0 ){
        set_case_message("fill:why:0", "data.fill(0)");
        result = false;
    }

    data.fill(0xaa);
    memset(buffer, 0xaa, 128);

    if( memcmp(data.data_const(), buffer, 128) != 0 ){
        set_case_message("fill:why:1", "data.fill(0xaa)");
        result = false;
    }

    return result;
}


bool DataTest::execute_alloc(){
    bool result = true;
    Data data;

    //test every API to see if it works as expected -- included giving it invalid values
    Data dynamic_data(512);

    if( dynamic_data.data() == 0 ){
        //failed to allocate memory
        set_case_message("why", "failed to allocate memory");
    }

    if( dynamic_data.data_const() == 0 ){
        //failed to allocate memory
        set_case_message("why", "data_const() failed");
    }


    return result;
}

bool DataTest::execute_class_performance_case(){
    bool result = true;
    int i;

    srand(8);

    for(i=0; i < 5000; i++){
        u32 data_size = rand() & 0xfff; //12 bits is up to 4096
        Data data(data_size);
        if( data.data() == 0 ){
            set_case_value("alloc failed", errno);
            break;
        }

        char buffer[data_size];
        memset(buffer, 0xaa, data_size);
        data.fill(0xaa);
        if( memcmp(buffer, data.data_const(), data_size) ){
            set_case_message("why", "memcmp failed");
        }
    }

    return result;
}



bool DataTest::execute_class_stress_case(){
    bool result = true;
    int i;

    srand(8);

    for(i=0; i < 5000; i++){
        u32 data_size = rand() & 0xfff; //12 bits is up to 4096
        Data data(data_size);
        if( data.data() == 0 ){
            set_case_value("alloc failed", errno);
            break;
        }

        char buffer[data_size];

        memset(buffer, 0xaa, data_size);
        data.fill(0xaa);
        if( memcmp(buffer, data.data_const(), data_size) ){
            set_case_message("why", "memcmp failed");
        }
    }

    return result;
}
