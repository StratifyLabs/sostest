#include "TransformQ31Test.hpp"

#include <sapi/dsp.hpp>
#include <sapi/chrono.hpp>

TransformQ31Test::TransformQ31Test() : Test("dsp::TransformQ31"){

}


bool TransformQ31Test::execute_class_api_case(){
    bool result = true;
    Timer t;

    SignalComplexQ31 data(1024);
    SignalComplexQ31 data1;

    //zero out complex values
    data.fill(0);

    u32 i;
    for(i=0; i < data.count(); i++){
        if( i % 2 ){
            data[i].real() = 10;
        } else {
            data[i].real() = -10;
        }
    }

    print_case_message("Construct FFT");
    RealFftQ31 fft(data.count());

    if( fft.error_number() != 0 ){
        result = false;
        print_case_message("Failed to construct FFT %d", data.count());
    } else {
        print_case_message("Perform FFT");
        t.start();
        data1 = data.transform(fft);
        t.stop();
        print_case_message("FFT in %ld microseconds", t.microseconds());
    }

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
