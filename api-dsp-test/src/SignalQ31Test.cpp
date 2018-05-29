#include "SignalQ31Test.hpp"

#include <sapi/chrono.hpp>
#include <sapi/dsp.hpp>


SignalQ31Test::SignalQ31Test() : Test("dsp::SignalQ31"){

}


bool SignalQ31Test::execute_class_api_case(){
    bool result = true;
    Timer t;
    u32 i;


    SignalQ31 data(32);
    SignalQ31 data1;
    SignalQ31 data2;

    if( data.request_arm_dsp_api() < 0 ){
        print_case_message("ARM DSP API is not available from the kernel");
        return false;
    } else {
        if( data.arm_dsp_api_q31() == 0 ){
            print_case_message("ARM DSP Q31 API is not available: abort");
            return false;
        }
    }

    if( data.count() != 32 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    for(i=0; i < data.count(); i++){
        if( i % 2 ){
            data[i] = 10;
        } else {
            data[i] = -10;
        }
    }

    if( data.mean() != 0 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    if( data.min() != -10 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    if( data.max() != 10 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    data1 = data.abs();
    if( data1.count() != 32 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    if( data1.min() != 10 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    if( data1.max() != 10 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    if( data1.mean() != 10 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    data1 = data + data;
    data2 = data + data;

    if( data1.mean() != 0 ){
        result = false;
        print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
    }

    if( data1.max() != 20 ){
        result = false;
        print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
    }

    if( data1.min() != -20 ){
        result = false;
        print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
    }

    if( data1 != data2 ){
        result = false;
        print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
    }

    data2[0] = 0;

    if( data1 == data2 ){
        result = false;
        print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
    }

    //data1 = data + data
    data2 = data1 - data;
    if( data2 != data ){
        result = false;
        print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
    }

    print_case_message("done");

    return result;
}

bool SignalQ31Test::execute_class_performance_case(){
    bool result = true;


    return result;
}

bool SignalQ31Test::execute_class_stress_case(){
    bool result = true;


    return result;
}
