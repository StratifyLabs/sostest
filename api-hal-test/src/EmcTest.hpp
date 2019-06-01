#ifndef EMC_TEST_HPP
#define EMC_TEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>


class EmcTest : public Test {
#define SRAM_BANK_ADDR                 (0x60000000)
/*on disco board accesible space only 4 mbit(524288 mbyte)*/
#define SRAM_SIZE                      (0x80000)
#define BUS_WIDTH 16
#define MAX_TEST_BUFFER_SIZE         512

public:
    EmcTest();
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 emc_test_count;
};

#endif // EMC_TEST_HPP
