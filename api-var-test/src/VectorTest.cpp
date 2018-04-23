#include "VectorTest.hpp"
#include <sapi/var.hpp>

VectorTest::VectorTest() : Test("var::Vector"){

}


bool VectorTest::execute_class_performance_case(){
    bool result = true;
    return result;
}

bool VectorTest::execute_class_stress_case(){
    bool result = true;
    return result;
}
/*! \details api test for var::Vector
 * size,push_back,pop_back,at,operator[],shrink_to_fit
 * @return false if same test failed
 */

bool VectorTest::execute_class_api_case(){
    bool result = true;
    {
        Vector <int>vector_test;
        u16 vector_size;
        if(vector_test.size()!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }
        vector_size = 2000;
        for (u16 i =0; i<vector_size; i++){
            vector_test.push_back(i);
            if(vector_test.count()!=(u16)(i+1)){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
        }
        vector_test.shrink_to_fit();
        for (u16 i =(vector_size-1); i; i--){
            if(vector_test.at(i)!=i){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
            if(vector_test[i]!=i){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
            vector_test.pop_back();
            if(vector_test.count()!=(u16)(i)){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
        }
        vector_test.pop_back(); //last
        if(vector_test.count()!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        vector_test.pop_back(); //we should be pop all value
        if(vector_test.count()!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
        vector_test.free();
    }
    {
        Vector <u16>vector_test;
        u16 vector_size;
        vector_size = 2000;
        for (u16 i =0; i<vector_size; i++){
            vector_test.push_back(i);
            if(vector_test.count()!=(u16)(i+1)){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
        }
//        print_case_message("count %lu",vector_test.count());
        for (u16 i =0; i<vector_size; i++){
            vector_test.insert(i+1,32768);
            if(vector_test.count()!=(u16)(vector_size+i+1)){
//                print_case_message("count %lu - %u",vector_test.count(),(u16)(vector_size+i+1));
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
        }
    }
    return result;
}
