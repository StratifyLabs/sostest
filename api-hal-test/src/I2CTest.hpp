#ifndef I2CTEST_HPP
#define I2CTEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>

class I2CTest : public Test {
public:
    I2CTest();
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 m_i2c_count;
    void get_i2c_count();
    bool execute_i2c_api_case(I2C & spi);
};

#endif // I2CTEST_HPP
