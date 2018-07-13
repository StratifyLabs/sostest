#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include "CliTest.hpp"
CliTest::CliTest() : Test("sys::Cli"){

}
static volatile bool stop_threads = false;
static bool thread_result = true;

/*@brief api test for sys/cli use "api-sys-test -cli -api"
 */
bool CliTest::execute_class_api_case(){
    bool result = true;
    return result;
}

/*@brief stress test for sys/cli use "api-sys-test -cli -stress"
 */
bool CliTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/Cli use "api-sys-test -cli -performance"
 */
bool CliTest::execute_class_performance_case(){
    bool result = true;

    return result;
}
void * CliTest::thread_1(u32 wait_time){
    int counter = 0;
    u16 test_count ;
    while( !stop_threads ){
        counter++;
        Timer::wait_microseconds(wait_time*1000);
        //add vector stress test from api-var-test(make it more difficult)
        Vector<int> vector_test;
        u16 vector_size;
        if(vector_test.size()!=0){
            thread_result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }
        test_count = rand()&0x3ff;
        vector_size = test_count + 1;
        for (u16 i =0; i<vector_size; i++){
            vector_test.push_back(i);
            vector_test.insert(i,vector_size-i);
            if(vector_test.count()!=(u16)(i*2+2)){
                print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                thread_result = false;
                break;
            }
        }
        vector_test.shrink_to_fit();
        for (u16 i = 0; i < vector_size*2; i++){
            if(i < vector_size){
                if(vector_test[i]!=(vector_size-i)){
                    print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                    thread_result = false;
                    break;
                }
            }else{
                if(vector_test[i]!=(i - vector_size)){
                    print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
                    thread_result = false;
                    break;
                }
            }
        }
        vector_test.free();
    }
    return &thread_result;
}
