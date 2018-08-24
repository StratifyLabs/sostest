#ifndef SPITEST_HPP
#define SPITEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>

class SpiTest : public Test {
public:
    SpiTest();
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 m_spi_count;
    void get_spi_count();
    bool execute_spi_api_case(Spi & spi);
};

#endif // SPITEST_HPP
