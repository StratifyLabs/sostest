#ifndef FILTER_TEST_HPP
#define FILTER_TEST_HPP

#include <sapi/test.hpp>
#include <sapi/sys.hpp>
#include <sapi/dsp.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

template<typename SignalType,
         typename BiquadFilterType,
         typename BiquadCoefficientsType> class FilterTest : public Test {
public:
    FilterTest(const char * name) : Test(name){}



    bool execute_class_api_case(){
        bool result = true;
        Timer t;
        u32 n_samples;
        u32 n_stages = 1;

        //change to FIFO style scheduling for more accurate performance comparisons
        Sched::set_scheduler(Sched::get_pid(), Sched::FIFO, 30);

        for(n_samples = 16; n_samples <= 512; n_samples += 16){
            SignalType data(n_samples);
            SignalType output(n_samples);
            BiquadCoefficientsType coefficients(n_stages);

            coefficients.b0(0) = 10;
            coefficients.b1(0) = 10;
            coefficients.b2(0) = 10;
            coefficients.a1(0) = 100;
            coefficients.a2(0) = 100;

            BiquadFilterType biquad_filter(coefficients);

            //Since we are in FIFO scheduler mode, we need to yield the processor
            Timer::wait_milliseconds(10);

            if( biquad_filter.error_number() != 0 ){
                result = false;
                print_case_message("Failed: %s, %d (%d)", __FILE__, __LINE__, biquad_filter.error_number());
            } else {
                String case_key;
                t.restart();
                data.filter(output, biquad_filter);
                t.stop();
                case_key.sprintf("microseconds-biquad-%d-%d", n_stages, n_samples);
                print_case_message_with_key(case_key.c_str(), "%ld", t.microseconds());

                t.restart();
                output = data.filter(biquad_filter);
                t.stop();
                case_key.sprintf("microseconds-alloc-biquad-%d-%d", n_stages, n_samples);
                print_case_message_with_key(case_key.c_str(), "%ld", t.microseconds());
            }
        }

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

};

class FilterQ15Test : public FilterTest<SignalQ15, BiquadFilterQ15, BiquadCoefficientsQ15> {
public:
    FilterQ15Test() : FilterTest("dsp::FilterQ15"){}
};

class FilterQ31Test : public FilterTest<SignalQ31, BiquadFilterQ31, BiquadCoefficientsQ31> {
public:
    FilterQ31Test() : FilterTest("dsp::FilterQ31"){

    }
};

class FilterF32Test : public FilterTest<SignalF32, BiquadFilterF32, BiquadCoefficientsF32> {
public:
    FilterF32Test() : FilterTest("dsp::FilterF32"){

    }
};

#endif // SIGNALTEST_HPP
