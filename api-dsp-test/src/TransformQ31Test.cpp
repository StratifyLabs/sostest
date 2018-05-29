#include "TransformQ31Test.hpp"

#include <sapi/dsp.hpp>
#include <sapi/chrono.hpp>

TransformQ31Test::TransformQ31Test() : Test("dsp::TransformQ31"){

}


bool TransformQ31Test::execute_class_api_case(){
    bool result = true;
    Timer t;
    u32 i;


    SignalQ31 data(32);
    SignalData<q31_t> data1;

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
        data[i] = 10;
    }

    t.start();
    if( data.mean() != 10 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }
    t.stop();
    print_case_message("dsp::SignalQ31::mean(%d): %ld microseconds", data.count(), t.microseconds());

    t.restart();
    data1 = data.abs();
    t.stop();
    print_case_message("dsp::SignalQ31::abs(%d) %ld microseconds", data.count(), t.microseconds());

    if( data1.count() != 32 ){
        result = false;
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
    }

    print_case_message("sizeof(Data) = %d", sizeof(var::Data));


    RfftQ31 rfft;

    rfft.init(data);

    //data.fft(rfft);


    return result;
}

bool TransformQ31Test::execute_class_performance_case(){
    bool result = true;


    return result;
}

bool TransformQ31Test::execute_class_stress_case(){
    bool result = true;


    return result;
}
