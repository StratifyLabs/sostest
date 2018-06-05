#include "TransformQ31Test.hpp"

#include <sapi/dsp.hpp>
#include <sapi/chrono.hpp>

TransformQ31Test::TransformQ31Test() : Test("dsp::TransformQ31"){

}


bool TransformQ31Test::execute_class_api_case(){
    bool result = true;
    Timer t;

    RealFftQ31 fft(128);
    SignalComplexQ31 data = fft.create_source_signal();
    SignalComplexQ31 data1 = fft.create_destination_signal();

    //zero out complex values
    data.fill(ComplexQ31(0,0));

    u32 i;
    for(i=0; i < data.count(); i++){
        if( i % 2 ){
            data[i].real() = 10;
        } else {
            data[i].real() = -10;
        }
    }

    print_case_message("Construct FFT");

    if( fft.error_number() != 0 ){
        result = false;
        print_case_message("Failed to construct FFT %d", data.count());
    } else {
        print_case_message("Perform FFT");
        t.start();
        data.transform(data1, fft);
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
