#ifndef EMATEST_HPP
#define EMATEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class EmaTest : public Test {
public:
    EmaTest();


private:
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // EMATEST_HPP
