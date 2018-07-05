#ifndef ADCTEST_HPP
#define ADCTEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>

class AdcTest : public Test {
public:
    AdcTest();

    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 m_adc_count;

    bool execute_adc_api_case(Adc & adc);

    void get_adc_count();
};

#endif // ADCTEST_HPP
