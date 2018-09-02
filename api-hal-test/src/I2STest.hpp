#ifndef I2STEST_HPP
#define I2STEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>

class I2STest : public Test {
public:
    I2STest();
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 m_i2s_count;
    void get_i2s_count();
    bool execute_i2s_api_case(I2S & spi);
};

#endif // I2STEST_HPP
