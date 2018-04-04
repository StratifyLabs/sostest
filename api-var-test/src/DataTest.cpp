
#include <cstdlib>
#include <sapi/sys.hpp>

#include "DataTest.hpp"
int DataTest::reqursive_number = 0;

DataTest::DataTest() : Test("var::Data"){
    reqursive_number = 0;

}


bool DataTest::execute_class_api_case(){
    bool result = true;

    if( execute_fill() == false ){
        print_case_message("why:0", "execute_fill()");
        result = false;
    }

    if( execute_alloc() == false ){
        print_case_message("why:0", "execute_alloc()");
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
        print_case_message("fill:why:0", "data.fill(0)");
        result = false;
    }

    data.fill(0xaa);
    memset(buffer, 0xaa, 128);

    if( memcmp(data.data_const(), buffer, 128) != 0 ){
        print_case_message("fill:why:1", "data.fill(0xaa)");
        result = false;
    }

    return result;
}


bool DataTest::execute_alloc(){
    bool result = true;
    char temp_string[] = "temp string";
    //test every API to see if it works as expected -- included giving it invalid values
    Data data;
    Data dynamic_data(512);
    Data exist_string(temp_string, sizeof(temp_string));

    if(data.data() != 0){
        print_case_message("why", "allocate memory exist");
    }
    if(exist_string.data() == 0 || (exist_string.capacity() != sizeof(temp_string))){
        print_case_message("why", "exist_string failed to allocate memory");
    }
    if(data.data_const() == 0){
        print_case_message("why", "data_const() failed");
    }
    if( dynamic_data.data() == 0 ){
        //failed to allocate memory
        print_case_message("why", "failed to allocate memory");
    }
    if( dynamic_data.data_const() == 0 || (dynamic_data.capacity() != 512)){
        //failed to allocate memory
        print_case_message("why", "data_const() failed");
    }
    return result;
}

bool DataTest::execute_class_performance_case(){
    bool result = true;
    int i;
    for(i=0; i < 5000; i++){
        u32 data_size = rand() & 0xfff; //12 bits is up to 4096
        Data data(data_size);
        if( data.data() == 0 ){
            print_case_message("alloc failed", errno);
            break;
        }

        char buffer[data_size];
        memset(buffer, 0xaa, data_size);
        data.fill(0xaa);
        if( memcmp(buffer, data.data_const(), data_size) ){
            print_case_message("why", "memcmp failed");
        }
        memset(buffer, 0x00, data_size);
		//add clear test
        data.clear();
        if( memcmp(buffer, data.data_const(), data_size) ){
            print_case_message("why", "memcmp failed");
        }
        if (data_size){
            char* t;
            t = data.cdata();
			//change one byte in data
            if (data_size < data.minimum_size()){
                t[data_size-1] = 0x0e;
            }else{
                t[data.calc_size()-1] = 0x0e;
            }
            if( !memcmp(buffer, data.data_const(), data_size) ){
                print_case_message("why", "memcmp not failed %d ",i);
            }
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
            print_case_message("alloc failed", errno);
            break;
        }
        char buffer[data_size];
        memset(buffer, 0xaa, data_size);
        data.fill(0xaa);
        if( memcmp(buffer, data.data_const(), data_size) ){
            print_case_message("why", "memcmp failed");
        }

    }
    Data data(145);
    if(!execute_reqursive(data)){
        print_case_message("req","reqursive test failed %d",reqursive_number);
    }else{
        print_case_message("req","reqursive test succesfull number%d",reqursive_number);
    }
    return result;
}

bool DataTest::execute_reqursive(Data data){
    reqursive_number++;
    if (data.calc_size() > data.minimum_size()){
        Data data_new(data.calc_size()-1);
        if( data_new.data() == 0 ){
            //failed to allocate memory
            return 0;
        }else{
            char fill_temp;
            fill_temp = reqursive_number;
            data_new.fill(fill_temp);
            if (!execute_reqursive(data_new)){
                return 0;
            }
            char buffer[data.calc_size()];
            memset(buffer, fill_temp, data_new.calc_size());
            if( memcmp(buffer, data_new.data_const(), data_new.calc_size()) ){
                //failed data
                char* t;
                t = data_new.cdata();
                print_case_message("why", "memcmp failed %d",fill_temp);
                print_case_message("data %d",t[0]);
                print_case_message("buffer %d",buffer[0]);
                return 0;
            }
        }
    }
    return 1;
}
