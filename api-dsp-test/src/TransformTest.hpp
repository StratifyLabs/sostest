#ifndef TRANSFORM_TEST_HPP
#define TRANSFORM_TEST_HPP

#include <sapi/test.hpp>
#include <sapi/sys.hpp>
#include <sapi/dsp.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

template<typename SignalType,
         typename FftComplexType,
         typename FftRealType> class TransformTest : public Test {
public:
    TransformTest(const char * name) : Test(name){}

    bool execute_class_api_case(){
        bool result = true;
        Timer t;

        //change to FIFO style scheduling for more accurate performance comparisons
        Sched::set_scheduler(Sched::get_pid(), Sched::FIFO, 30);

        api::DspWorkObject::request_arm_dsp_api();

        execute_real_fft(32);
        execute_real_fft(64);
        execute_real_fft(128);
        execute_real_fft(256);
        execute_real_fft(512);

        //restore the scheduling style
        Sched::set_scheduler(Sched::get_pid(), Sched::OTHER, 0);

        return result;
    }

    bool execute_class_performance_case(){

        return true;
    }


    bool execute_class_stress_case(){
        return true;
    }

private:
    bool execute_real_fft(u32 window_size){
        Timer t;
        bool result = true;
        FftRealType fft(window_size);

        if( fft.error_number() == 0 ){
            String case_key;
            SignalType data_time = fft.create_time_signal();
            SignalType data_frequency = fft.create_frequency_signal();

            t.restart();
            data_time.transform(data_frequency, fft);
            t.stop();
            case_key.sprintf("microseconds-fft-%d", window_size);
            print_case_message_with_key(case_key.c_str(), "%ld", t.microseconds());

            t.restart();
            data_frequency.transform(data_time, fft, true);
            t.stop();
            case_key.sprintf("microseconds-ifft-%d", window_size);
            print_case_message_with_key(case_key.c_str(), "%ld", t.microseconds());

        } else {
            result = false;
            print_case_message("Failed %s %d (%d)", __FILE__, __LINE__, fft.error_number());
        }
        Timer::wait_milliseconds(10); //yield the processor
        return result;
    }

};

class TransformQ15Test : public TransformTest<SignalComplexQ15, FftComplexQ15, FftRealQ15> {
public:
    TransformQ15Test() : TransformTest("dsp::TransformQ15"){}
};

class TransformQ31Test : public TransformTest<SignalComplexQ31, FftComplexQ31, FftRealQ31> {
public:
    TransformQ31Test() : TransformTest("dsp::TransformQ31"){

    }
};

class TransformF32Test : public TransformTest<SignalComplexF32, FftComplexF32, FftRealF32> {
public:
    TransformF32Test() : TransformTest("dsp::TransformF32"){

    }
};

#endif // SIGNALTEST_HPP
