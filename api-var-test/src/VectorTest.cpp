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
    const int test_count = 200;
    {
        Vector <int>vector_test;
        u16 vector_size;
        if(vector_test.size()!=0){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }
        vector_size = test_count;
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
        vector_size = test_count;
        for (u16 i =0; i < vector_size; i++){
            vector_test.push_back(i);
            if(vector_test.count()!=(u16)(i+1)){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                result = false;
                break;
            }
        }

        for (u16 i=0; i<vector_size; i++){
            if( vector_test.insert(i+1,32768) < 0 ){
                result = false;
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            }
            if(vector_test.count() != (u16)(vector_size+i+1)){
                print_case_message("Failed in cycle %s:%d:%d (%d != %d)", __PRETTY_FUNCTION__, __LINE__, i, vector_test.count(), vector_size+i+1);
                result = false;
                break;
            }
            if(vector_test.at(i+1) != 32768){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            }
        }

        if( vector_test.at(vector_test.count() - 1 ) != test_count-1 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        if( vector_test.count() != test_count*2 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

    }
    return result;
}
