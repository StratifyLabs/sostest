#ifndef FILTER_TEST_HPP
#define FILTER_TEST_HPP

#include <sapi/test.hpp>
#include <sapi/dsp.hpp>
#include <sapi/chrono.hpp>

template<typename SignalType,
         typename BiquadFilterType,
         typename BiquadCoefficientsType> class FilterTest : public Test {
public:
    FilterTest(const char * name) : Test(name){}

    bool execute_class_api_case(){
        bool result = true;

        SignalType data(16);
        BiquadCoefficientsType coefficients(1);
        coefficients.b0(0) = 0;
        coefficients.b1(0) = 0;
        coefficients.b2(0) = 0;
        coefficients.a1(0) = 0;
        coefficients.a2(0) = 0;
        BiquadFilterType biquad_filter(coefficients);

        if( biquad_filter.error_number() != 0 ){
            print_case_message("Failed: %s, %d", __FILE__, __LINE__);
        } else {
            data.filter(biquad_filter);
        }

        return result;
    }

    bool execute_class_performance_case(){

        return true;
    }


    bool execute_class_stress_case(){
        return true;
    }

};

class FilterQ15Test : public FilterTest<SignalQ15, BiquadDirectFormOneFilterQ15, BiquadCoefficientsQ15> {
public:
    FilterQ15Test() : FilterTest("dsp::FilterQ15"){

    }
};

class FilterQ31Test : public FilterTest<SignalQ31, BiquadDirectFormOneFilterQ31, BiquadCoefficientsQ31> {
public:
    FilterQ31Test() : FilterTest("dsp::FilterQ31"){

    }
};

class FilterF32Test : public FilterTest<SignalF32, BiquadDirectFormOneFilterF32, BiquadCoefficientsF32> {
public:
    FilterF32Test() : FilterTest("dsp::FilterF32"){

    }
};

#endif // SIGNALTEST_HPP
