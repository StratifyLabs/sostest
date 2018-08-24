#ifndef UARTTEST_HPP
#define UARTTEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>

class UartTest : public Test {
public:
    UartTest();
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 m_uart_count;
    void get_uart_count();
    bool execute_uart_api_case(Uart & uart);
};

#endif // UARTTEST_HPP
